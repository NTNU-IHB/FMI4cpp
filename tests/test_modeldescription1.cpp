
#define BOOST_TEST_MODULE ControlledTemperature_test1

#include <fmi4cpp/fmi2/fmi2.hpp>

#include <boost/test/unit_test.hpp>

#include <string>

using namespace fmi4cpp;

const std::string fmu_path = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                        "ControlledTemperature/modelDescription.xml";

BOOST_AUTO_TEST_CASE(ControlledTemperature_test1)
{
    auto md = fmi2::parse_model_description(fmu_path);
    auto md_cs = md->as_cs_description();

    BOOST_CHECK_EQUAL("2.0", md->fmi_version);
    BOOST_CHECK_EQUAL("ControlledTemperature", md->model_name);

    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md->guid);
    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md_cs->guid);
    BOOST_CHECK_EQUAL("20-sim", *md->generation_tool);
    BOOST_CHECK_EQUAL("20-sim", *md_cs->generation_tool);

    BOOST_CHECK_EQUAL(true, md->supports_cs());
    BOOST_CHECK_EQUAL(false, md->supports_me());

    BOOST_CHECK_EQUAL(120, md->model_variables->size());
    BOOST_CHECK_EQUAL(120, md_cs->model_variables->size());

    const fmi2::real_variable& heatCapacity1 = md->get_variable_by_name("HeatCapacity1.T0").as_real();
    BOOST_CHECK_EQUAL(1, heatCapacity1.valueReference());
    BOOST_CHECK_EQUAL(false, heatCapacity1.min().has_value());
    BOOST_CHECK_EQUAL(false, heatCapacity1.max().has_value());
    BOOST_CHECK_EQUAL(true, heatCapacity1.start().has_value());
    BOOST_CHECK_EQUAL(298.0, heatCapacity1.start().value());
    BOOST_CHECK_EQUAL("starting temperature", heatCapacity1.description());
    BOOST_CHECK_EQUAL(false, heatCapacity1.quantity().has_value());

    auto& thermalConductor = md->model_variables->getByValueReference(12);
    BOOST_CHECK_EQUAL("TemperatureSource.T", thermalConductor.name);
    BOOST_CHECK(fmi2::variability::tunable == thermalConductor.variability);
    BOOST_CHECK(fmi2::causality::parameter == thermalConductor.causality);

    fmi2::source_files sourceFiles = md_cs->sourceFiles;
    BOOST_CHECK_EQUAL(10, sourceFiles.size());
    BOOST_CHECK_EQUAL("EulerAngles.c", sourceFiles[0].name);

    std::vector<fmi2::unknown> modelStructureOutputs = md->model_structure->outputs;
    BOOST_CHECK_EQUAL(2, modelStructureOutputs.size());
    BOOST_CHECK_EQUAL(115, modelStructureOutputs[0].index);
    BOOST_CHECK_EQUAL(116, modelStructureOutputs[1].index);

    std::optional<fmi2::default_experiment> de = md->default_experiment;
    BOOST_CHECK(de.has_value());
    BOOST_CHECK_EQUAL(0.0, *de->startTime);
    BOOST_CHECK_EQUAL(20.0, *de->stopTime);
    BOOST_CHECK_EQUAL(1E-4, *de->stepSize);
    BOOST_CHECK_EQUAL(false, de->tolerance.has_value());

    size_t count = 0;
    auto mv = md->model_variables;
    for (const auto& v : *mv) {
        if (v.causality == fmi2::causality::output) {
            count++;
        }
    }
    BOOST_CHECK_EQUAL(2, count);

    std::vector<fmi2::scalar_variable> outputs = {};
    md->model_variables->getByCausality(fmi2::causality::output, outputs);

    BOOST_CHECK_EQUAL(count, outputs.size());
}
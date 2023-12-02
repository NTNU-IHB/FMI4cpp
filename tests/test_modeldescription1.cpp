
#include <fmi4cpp/fmi2/fmi2.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>

using namespace fmi4cpp;


TEST_CASE("ControlledTemperature_test1")
{
    const std::string fmu_path = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                                 "ControlledTemperature/modelDescription.xml";


    auto md = fmi2::parse_model_description(fmu_path);
    auto md_cs = md->as_cs_description();

    CHECK("2.0" == md->fmi_version);
    CHECK("ControlledTemperature" == md->model_name);

    CHECK("{06c2700b-b39c-4895-9151-304ddde28443}" == md->guid);
    CHECK("{06c2700b-b39c-4895-9151-304ddde28443}" == md_cs->guid);
    CHECK("20-sim" == *md->generation_tool);
    CHECK("20-sim" == *md_cs->generation_tool);

    CHECK(md->supports_cs());
    CHECK(!md->supports_me());

    CHECK(120 == md->model_variables->size());
    CHECK(120 == md_cs->model_variables->size());

    const fmi2::real_variable& heatCapacity1 = md->get_variable_by_name("HeatCapacity1.T0").as_real();
    CHECK(1 == heatCapacity1.valueReference());
    CHECK(!heatCapacity1.get_min().has_value());
    CHECK(!heatCapacity1.get_max().has_value());
    CHECK(heatCapacity1.start().has_value());
    CHECK(298.0 == Approx(heatCapacity1.start().value()));
    CHECK("starting temperature" == heatCapacity1.description());
    CHECK(!heatCapacity1.quantity().has_value());

    auto& thermalConductor = md->model_variables->getByValueReference(12);
    CHECK("TemperatureSource.T" == thermalConductor.name);
    CHECK(fmi2::variability::tunable == thermalConductor.variability);
    CHECK(fmi2::causality::parameter == thermalConductor.causality);

    fmi2::source_files sourceFiles = md_cs->sourceFiles;
    CHECK(10 == sourceFiles.size());
    CHECK("EulerAngles.c" == sourceFiles[0].name);

    std::vector<fmi2::unknown> modelStructureOutputs = md->model_structure->outputs;
    CHECK(2 == modelStructureOutputs.size());
    CHECK(115 == modelStructureOutputs[0].index);
    CHECK(116 == modelStructureOutputs[1].index);

    std::optional<fmi2::default_experiment> de = md->default_experiment;
    CHECK(de.has_value());
    CHECK(0.0 == Approx(*de->startTime));
    CHECK(20.0 == Approx(*de->stopTime));
    CHECK(1E-4 == Approx(*de->stepSize));
    CHECK(!de->tolerance.has_value());

    size_t count = 0;
    auto mv = md->model_variables;
    for (const auto& v : *mv) {
        if (v.causality == fmi2::causality::output) {
            count++;
        }
    }
    CHECK(2 == count);

    std::vector<fmi2::scalar_variable> outputs = {};
    md->model_variables->getByCausality(fmi2::causality::output, outputs);

    CHECK(count == outputs.size());
}
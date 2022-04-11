
#include <fmi4cpp/fmi2/fmi2.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>

using namespace fmi4cpp;

const double step_size = 1E-4;
const fmi2ValueReference vr = 46;

TEST_CASE("ControlledTemperature_test1")
{
    const std::string fmu_path = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                                 "ControlledTemperature/ControlledTemperature.fmu";

    auto fmu = fmi2::fmu(fmu_path).as_cs_fmu();

    size_t numOutputs = 0;
    for (const auto& v : *fmu->get_model_description()->model_variables) {
        if (v.causality == fmi2::causality::output) {
            numOutputs++;
        }
    }
    CHECK(2 == numOutputs);

    auto slave = fmu->new_instance();
    CHECK(slave->setup_experiment());
    CHECK(slave->enter_initialization_mode());
    CHECK(slave->exit_initialization_mode());

    double ref;
    CHECK(slave->step(step_size));
    CHECK(slave->read_real(vr, ref));

    CHECK(298.15 == Approx(ref));

    CHECK(slave->terminate());
}
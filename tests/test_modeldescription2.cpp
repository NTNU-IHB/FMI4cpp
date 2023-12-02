
#include <fmi4cpp/fmi2/fmi2.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <string>
#include <vector>

using namespace fmi4cpp::fmi2;

TEST_CASE("FmuExportCrossCompile_test1")
{

    const std::string fmu_path = "../resources/fmus/2.0/cs/OpenModelica/v1.11.0/"
                                 "FmuExportCrossCompile/modelDescription.xml";

    auto md = parse_model_description(fmu_path);

    CHECK("structured" == md->variable_naming_convention.value());

    std::vector<unknown> derivatives = md->model_structure->derivatives;
    CHECK(2 == derivatives.size());

    CHECK(3 == derivatives[0].index);
    CHECK(4 == derivatives[1].index);

    CHECK(true == derivatives[0].dependencies.has_value());
    CHECK(2 == derivatives[0].dependencies.value()[0]);
    REQUIRE(derivatives[0].dependencies_kind);
    REQUIRE(1 == derivatives[0].dependencies_kind.value().size());
    CHECK("dependent" == derivatives[0].dependencies_kind.value()[0]);
}

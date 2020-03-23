
#define BOOST_TEST_MODULE FmuExportCrossCompile_test1

#include <fmi4cpp/fmi2/fmi2.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

using namespace fmi4cpp::fmi2;

const std::string fmu_path = "../resources/fmus/2.0/cs/OpenModelica/v1.11.0/"
                        "FmuExportCrossCompile/modelDescription.xml";

BOOST_AUTO_TEST_CASE(FmuExportCrossCompile_test1)
{

    auto md = parse_model_description(fmu_path);

    BOOST_CHECK_EQUAL("structured", md->variable_naming_convention.value());

    std::vector<unknown> derivatives = md->model_structure->derivatives;
    BOOST_CHECK_EQUAL(2, derivatives.size());

    BOOST_CHECK_EQUAL(3, derivatives[0].index);
    BOOST_CHECK_EQUAL(4, derivatives[1].index);

    BOOST_CHECK_EQUAL(true, derivatives[0].dependencies.has_value());
    BOOST_CHECK_EQUAL(2, derivatives[0].dependencies.value()[0]);
    BOOST_CHECK_EQUAL("dependent", derivatives[0].dependencies_kind.value()[0]);
}

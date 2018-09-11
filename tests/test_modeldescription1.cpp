//
// Created by LarsIvar on 11.09.2018.
//

#define BOOST_TEST_MODULE MyTest

#include <string>
#include <boost/test/unit_test.hpp>
#include <fmicpp/tools/os_util.hpp>
#include <fmicpp/fmi2/fmicpp.hpp>


using namespace std;
using namespace fmicpp::fmi2;

const string fmu_path = string(getenv("TEST_FMUs"))
                        + "/FMI_2.0/CoSimulation/" + getOs() +
                        "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";


import::Fmu fmu(fmu_path);
auto md = fmu.getModelDescription();

BOOST_AUTO_TEST_CASE(test_equals) {


    BOOST_CHECK_EQUAL("2.0", md.fmiVersion);
    BOOST_CHECK_EQUAL("ControlledTemperature", md.modelName);
    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md.guid);
    BOOST_CHECK_EQUAL("20-sim", md.generationTool);

    BOOST_CHECK_EQUAL(true, md.supportsCoSimulation);
    BOOST_CHECK_EQUAL(false, md.supportsModelExchange);

}
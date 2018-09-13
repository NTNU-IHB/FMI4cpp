/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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

    BOOST_CHECK_EQUAL("2.0", md.fmiVersion());
    BOOST_CHECK_EQUAL("ControlledTemperature", md.modelName());
    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md.guid());
    BOOST_CHECK_EQUAL("20-sim", md.generationTool());

    BOOST_CHECK_EQUAL(true, md.supportsCoSimulation());
    BOOST_CHECK_EQUAL(false, md.supportsModelExchange());

    BOOST_CHECK_EQUAL(120, md.modelVariables().size());

    BOOST_CHECK_EQUAL("EulerAngles.c", md.asCoSimulationModelDescription()->sourceFiles().at(0).name());

}
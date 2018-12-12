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

#define BOOST_TEST_MODULE ControlledTemperature_Test


#include <string>
#include <iostream>
#include <boost/test/unit_test.hpp>

#include <fmi4cpp/tools/os_util.hpp>
#include <fmi4cpp/fmi2/fmi2.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;


const double stop = 10.0;
const double step_size = 1E-4;
const fmi2ValueReference vr = 46;

BOOST_AUTO_TEST_CASE(ControlledTemperature_test1) {

    const string fmu_path = string(getenv("TEST_FMUs"))
                            + "/2.0/cs/" + getOs() +
                            "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";

    auto fmu = fmi2Fmu(fmu_path).asCoSimulationFmu();

    size_t numOutputs = 0;
    for (const auto &v : *fmu->getModelDescription()->modelVariables()) {
        if (v.causality() == Causality::output) {
            numOutputs++;
        }
    }
    BOOST_CHECK_EQUAL(2, numOutputs);

    auto slave = fmu->newInstance();
    BOOST_CHECK(slave->setupExperiment());
    BOOST_CHECK(slave->enterInitializationMode());
    BOOST_CHECK(slave->exitInitializationMode());
    
    double ref;
    while ((slave->getSimulationTime()) <= (stop - step_size)) {
        BOOST_CHECK(slave->doStep(step_size));
        BOOST_CHECK(slave->readReal(vr, ref));
    }

    BOOST_CHECK(slave->terminate());
    
}
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

#include <string>

#include <fmi4cpp/fmi4cpp.hpp>
#include <fmi4cpp/common/logger.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

namespace logger = fmi4cpp::logger;

const double stop = 0.01;
const double stepSize = 1E-3;

const string fmuPath = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                       "ControlledTemperature/ControlledTemperature.fmu";

int main() {

    fmi2Fmu fmu(fmuPath);
    auto cs_fmu = fmu.asCoSimulationFmu();
    auto md = cs_fmu->getModelDescription();

    auto var = md->modelVariables->getByValueReference(47).asReal();
    logger::info("Name={}, start={}", var.name(), var.start().value_or(0));

    auto slave1 = cs_fmu->newInstance();
    auto slave2 = cs_fmu->newInstance();

    logger::info("modelIdentifier={}", slave1->getModelDescription()->modelIdentifier.data());

    slave1->setupExperiment();
    slave1->enterInitializationMode();
    slave1->exitInitializationMode();

    slave2->setupExperiment();
    slave2->enterInitializationMode();
    slave2->exitInitializationMode();

    vector<fmi2Real> ref(2);
    vector<fmi2ValueReference> vr = {md->getVariableByName("Temperature_Reference").valueReference,
                                     md->getVariableByName("Temperature_Room").valueReference};

    double t = 0;
    while ((t = slave1->getSimulationTime()) <= stop) {

        if (!slave1->doStep(stepSize)) { break; }
        if (!slave1->readReal(vr, ref)) { break; }
        logger::info("t={}, Temperature_Reference={}, Temperature_Room={}", t, ref[0], ref[1]);

    }

    logger::info("FMU '{}' terminated with success: {}", fmu.modelName(), (slave1->terminate() == 1 ? "true" : "false"));
    logger::info("FMU '{}' terminated with success: {}", fmu.modelName(), (slave2->terminate() == 1 ? "true" : "false"));

    return 0;

}
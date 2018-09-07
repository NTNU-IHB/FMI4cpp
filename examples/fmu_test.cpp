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
#include <iostream>
#include <fmicpp/tools/os_util.hpp>
#include <fmicpp/fmi2/fmicpp.hpp>

using namespace std;
using namespace fmicpp::fmi2;

int main() {

    const string fmuPath = string(getenv("TEST_FMUs"))
                            + "/FMI_2.0/CoSimulation/" + getOs() +
                            "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";

    import::Fmu fmu(fmuPath);

    cout << "Supports CoSimulation=" << fmu.supportsCoSimulation() << endl;
    cout << "Supports ModelExchange=" << fmu.supportsModelExchange() << endl;

    auto md = fmu.getModelDescription().asCoSimulationModelDescription();
    cout << "modelIdentifier=" << md->modelIdentifier << endl;
    cout << "Default stopTime=" << md->defaultExperiment.stopTime << endl;

    auto slave = fmu.asCoSimulationFmu().newInstance();
    slave->init();

    xml::ScalarVariable var = md->getVariableByValueReference(47);
    cout << "Causality=" << to_string(var.causality) << ", variability=" << to_string(var.variability) << endl;

    double t = 0;
    double stop = 1.0;
    double stepSize = 1E-3;

    fmi2Real v;
    fmi2Status status;
    while ((t = slave->getSimulationTime()) <= stop) {
        status = slave->doStep(stepSize);
        if (!status == fmi2OK) {
            break;
        }
        status = slave->readReal(var.valueReference, v);
        if (status != fmi2OK) {
            break;
        }
        cout << "t=" << t << ", " << var.name << "=" << v << endl;
    }

    status = slave->terminate();
    cout << "FMU '" << fmu.getModelName() <<  "' terminated with status: " << to_string(status) << endl;

    return 0;

}
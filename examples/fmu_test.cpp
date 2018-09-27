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
#include <fmi4cpp/tools/os_util.hpp>
#include <fmi4cpp/fmi2/fmi4cpp.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

int main() {

    const string fmuPath = string(getenv("TEST_FMUs"))
                           + "/FMI_2.0/CoSimulation/" + getOs() +
                           "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";

    import::Fmu fmu(fmuPath);

    cout << "Supports CoSimulation=" << fmu.supportsCoSimulation() << endl;
    cout << "Supports ModelExchange=" << fmu.supportsModelExchange() << endl;

    auto& md = fmu.getModelDescription();

    xml::ScalarVariable var = md.getVariableByValueReference(47);
    cout << var << endl;

    auto md_cs = md.asCoSimulationModelDescription();
    cout << "modelIdentifier=" << md_cs.getModelIdentifier() << endl;

    auto slave1 = fmu.asCoSimulationFmu().newInstance();
    auto slave2 = fmu.asCoSimulationFmu().newInstance();

    slave1->init();
    slave2->init();

    vector<fmi2Real> ref(2);
    vector<fmi2ValueReference> vr = {md.getVariableByName("Temperature_Reference").getValueReference(),
                                     md.getVariableByName("Temperature_Room").getValueReference()};

    double t = 0;
    double stop = 0.1;
    double stepSize = 1E-3;
    fmi2Status status;
    while ((t = slave1->getSimulationTime()) <= stop) {
        status = slave1->doStep(stepSize);
        if (status != fmi2OK) {
            break;
        }
        status = slave1->readReal(vr, ref);
        if (status != fmi2OK) {
            break;
        }
        cout << "t=" << t << ", Temperature_Reference=" << ref[0] << ", Temperature_Room=" << ref[1] << endl;
    }

    status = slave1->terminate();
    cout << "FMU '" << fmu.getModelName() << "' terminated with status: " << to_string(status) << endl;

    status = slave2->terminate();
    cout << "FMU '" << fmu.getModelName() << "' terminated with status: " << to_string(status) << endl;

    return 0;

}
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
#include <memory>
#include <iostream>
#include <vector>

#include <fmi4cpp/fmi2/fmi4cpp.hpp>
#include <fmi4cpp/tools/os_util.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

double stop = 1.0;
double microStep = 1E-3;
double macroStep = 1.0/10;

int main() {

    const string fmuPath = string(getenv("TEST_FMUs"))
                           + "/FMI_2.0/ModelExchange/" + getOs() +
                           "/OpenModelica/v1.11.0/FmuExportCrossCompile/FmuExportCrossCompile.fmu";

    auto fmu = Fmu(fmuPath).asModelExchangeFmu();
    unique_ptr<Solver> solver = make_unique<RK4Solver>(microStep);
    auto slave = fmu->newInstance(solver);

    auto hVar = slave->getModelDescription()->getVariableByName("h").asReal();

    fmi2Status status;
    status = slave->setupExperiment();
    status = slave->enterInitializationMode();
    status = slave->exitInitializationMode();

    double t = 0;
    double ref = 0;
    while ( ( t = slave->getSimulationTime()) <= stop) {

        if (!slave->doStep(macroStep) == fmi2OK) {
            break;
        }

        if (!hVar.read(*slave, ref) == fmi2OK) {
            break;
        }

        cout << "t=" << t << ", h=" << ref << endl;
    }

    slave->terminate();


}
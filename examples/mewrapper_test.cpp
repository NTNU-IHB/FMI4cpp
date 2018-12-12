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
#include <vector>
#include <iostream>

#include <fmi4cpp/fmi2/fmi2.hpp>
#include <fmi4cpp/tools/os_util.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;
using namespace fmi4cpp::solver;

namespace logger = fmi4cpp::logger;

const double stop = 1.0;
const double microStep = 1E-3;
const double macroStep = 1.0/10;

const string fmuPath = string(getenv("TEST_FMUs"))
                       + "/2.0/me/" + getOs() +
                       "/OpenModelica/v1.11.0/FmuExportCrossCompile/FmuExportCrossCompile.fmu";

int main() {
    
    auto fmu = fmi2Fmu(fmuPath).asModelExchangeFmu();

    auto solver = make_solver<RK4ClassicSolver>(microStep);
    auto slave = fmu->newInstance(solver);

    slave->setupExperiment();
    slave->enterInitializationMode();
    slave->exitInitializationMode();

    double t = 0;
    double ref = 0;
    auto hVar = slave->getModelDescription()->getVariableByName("h").asReal();

    while ( ( t = slave->getSimulationTime()) <= stop) {

        if (!slave->doStep(macroStep)) {
            logger::error("Error! doStep returned with status: {}", to_string(slave->getLastStatus()));
            break;
        }

        if (!hVar.read(*slave, ref)) {
            logger::error("Error! readReal returned with status: {}", to_string(slave->getLastStatus()));
            break;
        }

        logger::info("t={}, h={}",  t, ref);
    }

    slave->terminate();


}
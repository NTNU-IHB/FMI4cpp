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

#include <assert.h>
#include <string>

#include <fmi4cpp/fmi2/fmi2.hpp>
#include <fmi4cpp/common/logger.hpp>
#include <fmi4cpp/common/tools/os_util.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

const string fmu_path1 = "../resources/2.0/cs/20sim/4.6.4.8004/"
                         "TorsionBar/TorsionBar.fmu";

const string fmu_path2 = "../resources/2.0/cs/20sim/4.6.4.8004/"
                         "ControlledTemperature/ControlledTemperature.fmu";

int main() {
    
    fmi2Fmu fmu1(fmu_path1);
    fmi2Fmu fmu2(fmu_path2);
    
    const auto slave1 = fmu1.asCoSimulationFmu()->newInstance();
    const auto md1 = slave1->getModelDescription();

    slave1->setupExperiment();
    slave1->enterInitializationMode();
    slave1->exitInitializationMode();

    const auto slave2 = fmu2.asCoSimulationFmu()->newInstance();
    const auto md2 = slave2->getModelDescription();
    slave2->setupExperiment();
    slave2->enterInitializationMode();
    slave2->exitInitializationMode();

    slave1->doStep(1E-5);
    slave2->doStep(1E-4);

    double ref;
    auto var = md1->getVariableByName("MotorDiskRev").asReal();
    assert(var.valueReference() == 105);
    var.read(*slave1, ref);
    fmi4cpp::logger::info("MotorDiskRev={}", ref);

    auto vr = md2->getValueReference("Temperature_Room");
    assert(vr == 47);
    slave2->readReal(vr, ref);
    fmi4cpp::logger::info("Temperature_Room={}", ref);

    slave1->terminate();
    slave2->terminate();
    
}
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

#include <iostream>
#include <fmi4cpp/tools/os_util.hpp>
#include <fmi4cpp/fmi2/fmi4cpp.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

int main() {

    const string fmu_path1 = string(getenv("TEST_FMUs"))
                            + "/FMI_2.0/CoSimulation/" + getOs() +
                            "/20sim/4.6.4.8004/TorsionBar/TorsionBar.fmu";

    const string fmu_path2 = string(getenv("TEST_FMUs"))
                             + "/FMI_2.0/CoSimulation/" + getOs() +
                             "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";


    import::Fmu fmu1(fmu_path1);
    import::Fmu fmu2(fmu_path2);
    
    const auto slave1 = fmu1.asCoSimulationFmu()->newInstance();
    slave1->init();

    const auto slave2 = fmu2.asCoSimulationFmu()->newInstance();
    slave2->init();

    slave1->doStep(1E-5);
    slave2->doStep(1E-4);

    double ref;
    fmi2ValueReference vr = slave1->getValueReference("MotorDiskRev");
    assert(vr == 105);
    slave1->readReal(vr, ref);
    cout << "MotorDiskRev=" << ref << endl;

    vr = slave2->getValueReference("Temperature_Room");
    assert(vr == 47);
    slave2->readReal(vr, ref);
    cout << "Temperature_Room=" << ref << endl;

    slave1->terminate();
    slave2->terminate();
    
}
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
#include <iostream>

#include <fmi4cpp/fmi4cpp.hpp>

using namespace std;
using namespace fmi4cpp;

const string fmu_path1 = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                         "TorsionBar/TorsionBar.fmu";

const string fmu_path2 = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                         "ControlledTemperature/ControlledTemperature.fmu";

int main() {
    
    fmi2::fmu fmu1(fmu_path1);
    fmi2::fmu fmu2(fmu_path2);
    
    const auto slave1 = fmu1.as_cs_fmu()->new_instance();
    const auto md1 = slave1->get_model_description();

    slave1->setup_experiment();
    slave1->enter_initialization_mode();
    slave1->exit_initialization_mode();

    const auto slave2 = fmu2.as_cs_fmu()->new_instance();
    const auto md2 = slave2->get_model_description();
    slave2->setup_experiment();
    slave2->enter_initialization_mode();
    slave2->exit_initialization_mode();

    slave1->step(1E-5);
    slave2->step(1E-4);

    double ref;
    auto var = md1->get_variable_by_name("MotorDiskRev").as_real();
    assert(var.valueReference() == 105);
    var.read(*slave1, ref);
    cout << "MotorDiskRev=" << ref << endl;

    auto vr = md2->get_value_reference("Temperature_Room");
    assert(vr == 47);
    slave2->read_real(vr, ref);
    cout << "Temperature_Room=" << ref << endl;

    slave1->terminate();
    slave2->terminate();
    
    return 0;
    
}
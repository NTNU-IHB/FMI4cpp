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

#include <fmi4cpp/fmi4cpp.hpp>

#include <iostream>
#include <time_util.hpp>

using namespace fmi4cpp;

const double stop = 12.0;
const double step_size = 1E-5;
const fmi2ValueReference vr = 2;

int main()
{
    const std::string fmu_path = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                                 "TorsionBar/TorsionBar.fmu";

    fmi2::fmu fmu(fmu_path);

    const auto slave = fmu.as_cs_fmu()->new_instance();
    slave->setup_experiment();
    slave->enter_initialization_mode();
    slave->exit_initialization_mode();

    auto elapsed = measure_time_sec([&slave] {
        double ref;
        while ((slave->get_simulation_time()) <= (stop - step_size)) {
            if (!slave->step(step_size)) {
                std::cerr << "Error! step returned with status: " << to_string(slave->last_status()) << std::endl;
                break;
            }
            if (!slave->read_real(vr, ref)) {
                std::cerr << "Error! read_real returned with status: " << to_string(slave->last_status()) << std::endl;
                break;
            }
        }
    });

    std::cout << "Time elapsed=" << elapsed << "s" << std::endl;

    slave->terminate();

    return 0;
}
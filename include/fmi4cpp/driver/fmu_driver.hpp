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

#ifndef FMI4CPP_FMUDRIVER_HPP
#define FMI4CPP_FMUDRIVER_HPP

#include <fmi4cpp/driver/driver_options.hpp>
#include <fmi4cpp/driver/error_types.hpp>
#include <fmi4cpp/fmi4cpp.hpp>

#include <memory>
#include <string>


namespace fmi4cpp::driver
{

class fmu_driver
{

public:
    fmu_driver(const std::string& fmuPath, const driver_options& options);

    void run();

private:
    const std::string& fmuPath_;
    const driver_options& options_;

    void dump_output(const std::string& data, const std::string& outputFolder);

    void simulate(std::unique_ptr<fmu_slave<fmi2::cs_model_description>> slave);
};

} // namespace fmi4cpp::driver


#endif //FMI4CPP_FMUDRIVER_HPP

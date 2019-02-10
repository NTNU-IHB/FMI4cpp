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

#include <string>
#include <memory>

#include <fmi4cpp/fmi4cpp.hpp>

#include "error_types.hpp"
#include "driver_options.hpp"


namespace fmi4cpp::driver {

    class fmu_driver {

    public:

        explicit fmu_driver(const std::shared_ptr<fmi4cpp::fmi2::fmi2Fmu> fmu);

        void run(driver_options options);

    private:

        const std::shared_ptr<fmi4cpp::fmi2::fmi2Fmu> fmu_;

        void dumpOutput(const std::string &data, const std::string &outputFolder);

        void simulate(std::unique_ptr<fmi4cpp::FmuSlave<fmi4cpp::fmi2::CoSimulationModelDescription>> slave,
                      driver_options options);

    };

}


#endif //FMI4CPP_FMUDRIVER_HPP

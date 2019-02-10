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

#ifndef FMI4CPP_DRIVEROPTIONS_HPP
#define FMI4CPP_DRIVEROPTIONS_HPP

#include <experimental/filesystem>

#include <fmi4cpp/fmi2/xml/ScalarVariable.hpp>

namespace fs = std::experimental::filesystem;

namespace fmi4cpp::driver {

    struct driver_options {

        double startTime = 0.0;
        double stopTime = 0.0;
        double stepSize = 1e-3;

        bool modelExchange = false;
        bool failOnLargeFileSize = false;

        fs::path outputFolder = fs::current_path();
        std::vector<fmi4cpp::fmi2::ScalarVariable> variables;

    };

}

#endif //FMI4CPP_DRIVEROPTIONS_HPP

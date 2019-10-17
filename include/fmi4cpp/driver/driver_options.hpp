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

#include <fmi4cpp/fmi2/xml/model_description.hpp>
#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>

#include <boost/filesystem.hpp>

#include <algorithm>

namespace fmi4cpp::driver
{

struct driver_options
{

    double startTime = 0.0;
    double stopTime = 0.0;
    double stepSize = 1e-3;

    bool modelExchange = false;
    bool failOnLargeFileSize = false;

    std::vector<std::string> variables;
    boost::filesystem::path outputFolder = boost::filesystem::current_path();

    std::vector<fmi2::scalar_variable>
    transformVariables(std::shared_ptr<const fmi2::model_description_base> md) const
    {
        std::vector<fmi2::scalar_variable> result;
        std::transform(variables.begin(), variables.end(), std::back_inserter(result),
            [md](const std::string& name) -> fmi2::scalar_variable {
                return md->get_variable_by_name(name);
            });
        return result;
    }
};

} // namespace fmi4cpp::driver

#endif //FMI4CPP_DRIVEROPTIONS_HPP

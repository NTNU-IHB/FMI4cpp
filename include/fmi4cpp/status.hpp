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

#ifndef FMI4CPP_STATUS_HPP
#define FMI4CPP_STATUS_HPP

#include <string>

namespace fmi4cpp
{

enum class status
{
    OK,
    Warning,
    Discard,
    Error,
    Fatal,
    Pending,
    Unknown
};

const inline std::string to_string(fmi4cpp::status status)
{
    switch (status) {
        case fmi4cpp::status::OK:
            return "OK";
        case fmi4cpp::status::Warning:
            return "Warning";
        case fmi4cpp::status::Discard:
            return "Discard";
        case fmi4cpp::status::Error:
            return "Error";
        case fmi4cpp::status::Fatal:
            return "Fatal";
        case fmi4cpp::status::Pending:
            return "Pending";
        default:
            return "Unknown";
    }
}

} // namespace fmi4cpp

#endif //FMI4CPP_STATUS_HPP

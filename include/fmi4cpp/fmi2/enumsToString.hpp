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

#ifndef FMI4CPP_ENUMSTOSTRING_HPP
#define FMI4CPP_ENUMSTOSTRING_HPP

#include <string>
#include "xml/enums.hpp"
#include "fmi2FunctionTypes.h"

namespace {

    const std::string to_string(fmi2Status status) {

        switch (status) {
            case fmi2OK:
                return "OK";
            case fmi2Warning:
                return "Warning";
            case fmi2Discard:
                return "Discard";
            case fmi2Error:
                return "Error";
            case fmi2Fatal:
                return "Fatal";
            case fmi2Pending:
                return "Pending";
            default:
                return "Unknown";
        }

    }


    const std::string to_string(fmi4cpp::fmi2::Causality causality) {

        switch (causality) {
            case fmi4cpp::fmi2::Causality::calculatedParameter:
                return "calculatedParameter";
            case fmi4cpp::fmi2::Causality::parameter:
                return "parameter";
            case fmi4cpp::fmi2::Causality::input:
                return "input";
            case fmi4cpp::fmi2::Causality::output:
                return "output";
            case fmi4cpp::fmi2::Causality::local:
                return "local";
            case fmi4cpp::fmi2::Causality::independent:
                return "independent";
            default:
                return "unknown";
        }

    }

    const std::string to_string(fmi4cpp::fmi2::Variability variability) {

        switch (variability) {
            case fmi4cpp::fmi2::Variability::constant:
                return "constant";
            case fmi4cpp::fmi2::Variability::fixed:
                return "fixed";
            case fmi4cpp::fmi2::Variability::tunable:
                return "tunable";
            case fmi4cpp::fmi2::Variability::discrete:
                return "discrete";
            case fmi4cpp::fmi2::Variability::continuous:
                return "continuous";
            default:
                return "unknown";
        }

    }

    const std::string to_string(fmi4cpp::fmi2::Initial initial) {
        switch (initial) {
            case fmi4cpp::fmi2::Initial::exact:
                return "exact";
            case fmi4cpp::fmi2::Initial::approx:
                return "approx";
            case fmi4cpp::fmi2::Initial::calculated:
                return "calculated";
            case fmi4cpp::fmi2::Initial::unknown:
            default:
                return "unknown";
        }
    }

}

#endif //FMI4CPP_ENUMSTOSTRING_HPP

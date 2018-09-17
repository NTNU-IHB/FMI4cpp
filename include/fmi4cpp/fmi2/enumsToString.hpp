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
#include <stdexcept>

#include "fmi2Functions.h"
#include "xml/enums.hpp"

using std::string;
using namespace fmi4cpp::fmi2::xml;

namespace {

    const string to_string(fmi2Status status) {

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
                throw std::runtime_error("Invalid fmi2Status encountered!");
        }

    }

    const string to_string(fmi2Causality causality) {

        switch (causality) {
            case fmi2Causality::calculatedParameter:
                return "Warning";
            case fmi2Causality::input:
                return "input";
            case fmi2Causality::output:
                return "output";
            case fmi2Causality::local:
                return "local";
            case fmi2Causality::independent:
                return "independent";
            default:
                throw std::runtime_error("Invalid fmi2Causality encountered");
        }

    }

    const string to_string(fmi2Variability variability) {

        switch (variability) {
            case fmi2Variability::constant:
                return "constant";
            case fmi2Variability::fixed:
                return "fixed";
            case fmi2Variability::tunable:
                return "tunable";
            case fmi2Variability::discrete:
                return "discrete";
            case fmi2Variability::continuous:
                return "continuous";
            default:
                throw std::runtime_error("Invalid fmi2Variability encountered");
        }

    }

    const string to_string(fmi2Initial initial) {

        switch (initial) {
            case fmi2Initial::exact:
                return "exact";
            case fmi2Initial::approx:
                return "approx";
            case fmi2Initial::calculated:
                return "calculated";
            default:
                throw std::runtime_error("Invalid fmi2Initial encountered");
        }

    }

}

#endif //FMI4CPP_ENUMSTOSTRING_HPP

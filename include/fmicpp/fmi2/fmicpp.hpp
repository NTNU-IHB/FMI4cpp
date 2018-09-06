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

#ifndef FMICPP_FMICPP_HPP
#define FMICPP_FMICPP_HPP

#include <string>
#include "fmi2Functions.h"

#include "xml/ModelDescription.hpp"
#include "xml/ScalarVariable.hpp"

#include "import/Fmu.hpp"
#include "import/FmuInstance.hpp"
#include "import/FmuSlave.hpp"

#include "import/CoSimulationSlaveBuilder.hpp"

namespace {

    std::string to_string(fmi2Status status) {

        switch (status) {
            case fmi2OK: return "OK";
            case fmi2Warning: return "Warning";
            case fmi2Discard: return "Discard";
            case fmi2Error: return "Error";
            case fmi2Fatal: return "Fatal";
            case fmi2Pending: return "Pending";
            default: throw std::runtime_error(std::string("ERROR: Not a valid status:") + std::to_string(status) + "!");
        }

    }

    std::string to_string(fmicpp::fmi2::xml::Causality causality) {

        using fmicpp::fmi2::xml::Causality;

        switch (causality) {
            case  Causality::calculatedParameter: return "Warning";
            case  Causality::input: return "input";
            case  Causality::output: return "output";
            case  Causality::local: return "local";
            case  Causality::independent: return "independent";
        }

    }

    std::string to_string(fmicpp::fmi2::xml::Variability variability) {

        using fmicpp::fmi2::xml::Variability;

        switch (variability) {
            case  Variability::constant: return "constant";
            case  Variability::fixed: return "fixed";
            case  Variability::tunable: return "tunable";
            case  Variability::discrete: return "discrete";
            case  Variability::continuous: return "continuous";
        }

    }

}

#endif //FMICPP_FMICPP_HPP

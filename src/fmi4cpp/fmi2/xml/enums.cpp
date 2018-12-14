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

#include <fmi4cpp/fmi2/xml/enums.hpp>

using fmi4cpp::fmi2::Causality;
using fmi4cpp::fmi2::Variability;
using fmi4cpp::fmi2::Initial;

Causality fmi4cpp::fmi2::parseCausality(const std::string &str) {
    if (str == "calculatedParameter") {
        return Causality::calculatedParameter;
    } else if (str == "parameter") {
        return Causality::parameter;
    } else if (str == "input") {
        return Causality::input;
    } else if (str == "output") {
        return Causality::output;
    } else if (str == "local") {
        return Causality::local;
    } else if (str == "independent") {
        return Causality::independent;
    } else {
        return Causality::local;
    }
}

Variability fmi4cpp::fmi2::parseVariability(const std::string &str) {
    if (str == "constant") {
        return Variability::constant;
    } else if (str == "fixed") {
        return Variability::fixed;
    } else if (str == "tunable") {
        return Variability::tunable;
    } else if (str == "discrete") {
        return Variability::discrete;
    } else if (str == "continuous") {
        return Variability::continuous;
    } else {
        return Variability::continuous;
    }
}

Initial fmi4cpp::fmi2::parseInitial(const std::string &str) {
    if (str == "exact") {
        return Initial::exact;
    } else if (str == "approx") {
        return Initial::approx;
    } else if (str == "calculated") {
        return Initial::calculated;
    } else {
        return Initial::unknown;
    }

}

const std::string fmi4cpp::fmi2::to_string(Causality causality) {

    switch (causality) {
        case Causality::calculatedParameter:
            return "calculatedParameter";
        case Causality::parameter:
            return "parameter";
        case Causality::input:
            return "input";
        case Causality::output:
            return "output";
        case Causality::local:
            return "local";
        case Causality::independent:
            return "independent";
        case Causality::unknown:
        default:
            return "unknown";
    }

}

const std::string fmi4cpp::fmi2::to_string(Variability variability) {

    switch (variability) {
        case Variability::constant:
            return "constant";
        case Variability::fixed:
            return "fixed";
        case Variability::tunable:
            return "tunable";
        case Variability::discrete:
            return "discrete";
        case Variability::continuous:
            return "continuous";
        case Variability::unknown:
        default:
            return "unknown";
    }

}

const std::string fmi4cpp::fmi2::to_string(Initial initial) {
    switch (initial) {
        case Initial::exact:
            return "exact";
        case Initial::approx:
            return "approx";
        case Initial::calculated:
            return "calculated";
        case Initial::unknown:
        default:
            return "unknown";
    }
}

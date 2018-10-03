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

using fmi4cpp::fmi2::xml::fmi2Causality;
using fmi4cpp::fmi2::xml::fmi2Variability;
using fmi4cpp::fmi2::xml::fmi2Initial;

fmi2Causality fmi4cpp::fmi2::xml::parseCausality(const std::string &str) {
    if (str == "calculatedParameter") {
        return fmi2Causality::calculatedParameter;
    } else if (str == "parameter") {
        return fmi2Causality::parameter;
    } else if (str == "input") {
        return fmi2Causality::input;
    } else if (str == "output") {
        return fmi2Causality::output;
    } else if (str == "local") {
        return fmi2Causality::local;
    } else if (str == "independent") {
        return fmi2Causality::independent;
    } else {
        return fmi2Causality::local;
    }
}

fmi2Variability fmi4cpp::fmi2::xml::parseVariability(const std::string &str) {
    if (str == "constant") {
        return fmi2Variability::constant;
    } else if (str == "fixed") {
        return fmi2Variability::fixed;
    } else if (str == "tunable") {
        return fmi2Variability::tunable;
    } else if (str == "discrete") {
        return fmi2Variability::discrete;
    } else if (str == "continuous") {
        return fmi2Variability::continuous;
    } else {
        return fmi2Variability::continuous;
    }
}

fmi2Initial fmi4cpp::fmi2::xml::parseInitial(const std::string &str) {
    if (str == "exact") {
        return fmi2Initial::exact;
    } else if (str == "approx") {
        return fmi2Initial::approx;
    } else if (str == "calculated") {
        return fmi2Initial::calculated;
    } else {
        return fmi2Initial::unknown;
    }

}

const std::string fmi4cpp::fmi2::xml::to_string(fmi2Causality causality) {

    switch (causality) {
        case fmi2Causality::calculatedParameter:
            return "calculatedParameter";
        case fmi2Causality::parameter:
            return "parameter";
        case fmi2Causality::input:
            return "input";
        case fmi2Causality::output:
            return "output";
        case fmi2Causality::local:
            return "local";
        case fmi2Causality::independent:
            return "independent";
        default:
            return "unknown";
    }

}

const std::string fmi4cpp::fmi2::xml::to_string(fmi2Variability variability) {

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
            return "unknown";
    }

}

const std::string fmi4cpp::fmi2::xml::to_string(fmi2Initial initial) {
    switch (initial) {
        case fmi2Initial::exact:
            return "exact";
        case fmi2Initial::approx:
            return "approx";
        case fmi2Initial::calculated:
            return "calculated";
        case fmi2Initial::unknown:
        default:
            return "unknown";
    }
}

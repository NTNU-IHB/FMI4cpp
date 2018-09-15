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

#include <fmicpp/fmi2/xml/DefaultExperiment.hpp>

using namespace fmicpp::fmi2::xml;

void DefaultExperiment::load(const ptree &node) {
    startTime_ = node.get_optional<double>("<xmlattr>.startTime");
    stopTime_ = node.get_optional<double>("<xmlattr>.stopTime");
    stepSize_ = node.get_optional<double>("<xmlattr>.stepSize");
    tolerance_ = node.get_optional<double>("<xmlattr>.tolerance");
}

boost::optional<double> DefaultExperiment::startTime() const {
    return startTime_;
}

boost::optional<double> DefaultExperiment::stopTime() const {
    return stopTime_;
}

boost::optional<double> DefaultExperiment::stepSize() const {
    return stepSize_;
}

boost::optional<double> DefaultExperiment::tolerance() const {
    return tolerance_;
}

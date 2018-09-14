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
    auto startTime_optional = node.get_optional<double>("<xmlattr>.startTime");
    if (startTime_optional) {
        startTime_ = std::make_shared<double>(*startTime_optional);
    }
    auto stopTime_optional = node.get_optional<double>("<xmlattr>.stopTime");
    if (stopTime_optional) {
        stopTime_ = std::make_shared<double>(*stopTime_optional);
    }
    auto stepSize_optional = node.get_optional<double>("<xmlattr>.stepSize");
    if (stepSize_optional) {
        stepSize_ = std::make_shared<double>(*stepSize_optional);
    }
    auto tolerance_optional = node.get_optional<double>("<xmlattr>.tolerance");
    if (tolerance_optional) {
        tolerance_ = std::make_shared<double>(*tolerance_optional);
    }
}

std::shared_ptr<double> DefaultExperiment::startTime() const {
    return startTime_;
}

std::shared_ptr<double> DefaultExperiment::stopTime() const {
    return stopTime_;
}

std::shared_ptr<double> DefaultExperiment::stepSize() const {
    return stepSize_;
}

std::shared_ptr<double> DefaultExperiment::tolerance() const {
    return tolerance_;
}

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

#include <stdexcept>
#include <fmi4cpp/fmi2/xml/enums.hpp>
#include <fmi4cpp/fmi2/xml/ModelVariables.hpp>

using namespace fmi4cpp::fmi2::xml;

ModelVariables::ModelVariables() {}

ModelVariables::ModelVariables(const std::vector<ScalarVariable> &variables): variables_(variables) {}

const ScalarVariable &ModelVariables::getByName(const std::string &name) const {
    for (auto &v : variables_) {
        if (v.name() == name) {
            return v;
        }
    }
    throw std::runtime_error("No such variable with name '" + name + "'!");
}

const ScalarVariable &ModelVariables::getByValueReference(const fmi2ValueReference vr) const {
    for (const auto &var : variables_) {
        if (var.valueReference() == vr) {
            return var;
        }
    }
    throw std::runtime_error("No such variable with valueReference '" + std::to_string(vr) + "'!");
}

void ModelVariables::getByCausality(const fmi2Causality causality, std::vector<ScalarVariable> &store) const {
    for (const auto &var : variables_) {
        if (var.causality() == causality) {
            store.push_back(var);
        }
    }
}

size_t ModelVariables::size() const {
    return variables_.size();
}
 const ScalarVariable &ModelVariables::operator[](const size_t index) const {
    return variables_[index];
}

std::vector<ScalarVariable>::iterator ModelVariables::begin() {
    return variables_.begin();
}
std::vector<ScalarVariable>::iterator ModelVariables::end() {
    return variables_.end();
}

std::vector<ScalarVariable>::const_iterator ModelVariables::cbegin() const{
    return variables_.cbegin();
}
std::vector<ScalarVariable>::const_iterator ModelVariables::cend() const {
    return variables_.cend();
}

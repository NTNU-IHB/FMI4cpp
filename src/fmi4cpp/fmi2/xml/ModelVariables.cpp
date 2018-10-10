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
#include <fmi4cpp/fmi2/xml/ScalarVariableImpl.hpp>
#include <fmi4cpp/fmi2/xml/ModelVariables.hpp>

using namespace std;
using namespace fmi4cpp::fmi2::xml;

ModelVariables::ModelVariables(const vector<shared_ptr<ScalarVariable>> &variables_) : variables_(variables_) {}


//ModelVariables::ModelVariables(const ptree &node) {
//    for (const ptree::value_type &v : node) {
//        if (v.first == "ScalarVariable") {
//            variables_.push_back(make_shared<ScalarVariableImpl>(v.second));
//        }
//    }
//}

const ScalarVariable &ModelVariables::getByName(const string &name) const {
    for (const auto &var : variables_) {
        if (var->getName() == name) {
            return *var;
        }
    }
    throw runtime_error("No such variable with name '" + name + "'!");
}

const ScalarVariable &ModelVariables::getByValueReference(const fmi2ValueReference vr) const {
    for (const auto &var : variables_) {
        if (var->getValueReference() == vr) {
            return *var;
        }
    }
    throw runtime_error("No such variable with valueReference '" + std::to_string(vr) + "'!");
}

void ModelVariables::getByCausality(const fmi2Causality causality, vector<std::reference_wrapper<ScalarVariable>> &store) const {
    for (const auto &var : variables_) {
        if (var->getCausality() == causality) {
            store.push_back(std::ref(*var));
        }
    }
}

size_t ModelVariables::size() const {
    return variables_.size();
}
 const ScalarVariable &ModelVariables::operator[](const size_t index) const {
    return *variables_[index];
}
 vector<shared_ptr<ScalarVariable>>::iterator ModelVariables::begin() {
    return variables_.begin();
}
 vector<shared_ptr<ScalarVariable>>::iterator ModelVariables::end() {
    return variables_.end();
}
 vector<shared_ptr<ScalarVariable>>::const_iterator ModelVariables::cbegin() const{
    return variables_.cbegin();
}
 vector<shared_ptr<ScalarVariable>>::const_iterator ModelVariables::cend() const {
    return variables_.cend();
}


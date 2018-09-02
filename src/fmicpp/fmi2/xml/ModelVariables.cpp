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

#include <fmicpp/fmi2/xml/ModelVariables.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;


void fmicpp::fmi2::xml::ModelVariables::load(const ptree &node) {

    for (const ptree::value_type &v : node) {

        if (v.first == "ScalarVariable") {
            unique_ptr<ScalarVariable> var(new ScalarVariable());
            var->load(v.second);
            push_back(move(var));
        }

    }

}

ScalarVariable& ModelVariables::getByName(const string name) {

    for (const auto& var : *this) {
        if (var->name == name) {
            return *var;
        }
    }

    string errorMsg = "No such variable with name '" + name + "'!";
    throw runtime_error(errorMsg);
}

ScalarVariable& ModelVariables::getByValueReference(const fmi2ValueReference vr) {

    for (const auto& var : *this) {
        if (var->valueReference == vr) {
            return *var;
        }
    }

    string errorMsg = "No such variable with valueReference '" + to_string(vr) + "'!";
    throw runtime_error(errorMsg);
}

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

#include <fmicpp/fmi2/xml/ScalarVariable.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;

void IntegerAttribute::load(ptree &node) {
    min = node.get<int>("<xmlattr>.min", 0);
    max = node.get<int>("<xmlattr>.max", 0);
    start = node.get<int>("<xmlattr>.start", 0);
}

void RealAttribute::load(ptree &node) {
    min = node.get<double>("<xmlattr>.min", 0);
    max = node.get<double>("<xmlattr>.max", 0);
    start = node.get<double>("<xmlattr>.start", 0);
}

void StringAttribute::load(ptree &node) {
    start = node.get<string>("<xmlattr>.start", "");
}

void BooleanAttribute::load(ptree &node) {
    start = node.get<bool>("<xmlattr>.start", false);
}

void ScalarVariable::load(ptree &node) {

    valueReference = node.get<ValueReference>("<xmlattr>.valueReference");
    name = node.get<string>("<xmlattr>.name");
    description = node.get<string>("<xmlattr>.description", "");

    for (ptree::value_type &v : node) {
        if (v.first == "Integer") {
            integerAttribute = make_unique<IntegerAttribute>(IntegerAttribute());
            integerAttribute->load(v.second);
        } else if (v.first == "Real") {
            realAttribute = make_unique<RealAttribute>(RealAttribute());
            realAttribute->load(v.second);
        } else if (v.first == "String") {
            stringAttribute = make_unique<StringAttribute>(StringAttribute());
            stringAttribute->load(v.second);
        } else if (v.first == "Boolean") {
            booleanAttribute = make_unique<BooleanAttribute>(BooleanAttribute());
            booleanAttribute->load(v.second);
        }
    }

}

IntegerVariable ScalarVariable::asIntegerVariable() {
    return IntegerVariable(*integerAttribute);
}

RealVariable ScalarVariable::asRealVariable() {
    return RealVariable(*realAttribute);
}

IntegerVariable::IntegerVariable(IntegerAttribute &attribute) {
    this->min = attribute.min;
    this->max = attribute.max;
    this->start = attribute.start;
}

RealVariable::RealVariable(RealAttribute &attribute) {
    this->min = attribute.min;
    this->max = attribute.max;
    this->start = attribute.start;
}

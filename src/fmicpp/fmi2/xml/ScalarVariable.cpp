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

void IntegerAttribute::load(const ptree &node) {
    min = node.get<int>("<xmlattr>.min", 0);
    max = node.get<int>("<xmlattr>.max", 0);
    start = node.get<int>("<xmlattr>.start", 0);
    quantity = node.get<string>("<xmlattr>.quantity", "");
}

void RealAttribute::load(const ptree &node) {
    min = node.get<double>("<xmlattr>.min", 0);
    max = node.get<double>("<xmlattr>.max", 0);
    start = node.get<double>("<xmlattr>.start", 0);
    nominal = node.get<double>("<xmlattr>.nominal", 0);

    quantity = node.get<string>("<xmlattr>.quantity", "");
    unit = node.get<string>("<xmlattr>.unit", "");
    displayUnit = node.get<string>("<xmlattr>.displayUnit", "");

    reinit = node.get<bool>("<xmlattr>.reinit", false);
    unbounded = node.get<bool>("<xmlattr>.unbounded", false);
    relativeQuantity = node.get<bool>("<xmlattr>.relativeQuantity", false);

    derivative = node.get<unsigned int>("<xmlattr>.reinit", -1);
}

void StringAttribute::load(const ptree &node) {
    start = node.get<string>("<xmlattr>.start", "");
}

void BooleanAttribute::load(const ptree &node) {
    start = node.get<bool>("<xmlattr>.start", false);
}

void EnumerationAttribute::load(const ptree &node) {
    min = node.get<int>("<xmlattr>.min", 0);
    max = node.get<int>("<xmlattr>.max", 0);
    start = node.get<int>("<xmlattr>.start", 0);
    quantity = node.get<string>("<xmlattr>.quantity", "");
}

void ScalarVariable::load(const ptree &node) {

    valueReference = node.get<fmi2ValueReference >("<xmlattr>.valueReference");
    name = node.get<string>("<xmlattr>.name");
    description = node.get<string>("<xmlattr>.description", "");
    canHandleMultipleSetPerTimelnstant = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

    for (const ptree::value_type &v : node) {
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
        } else if (v.first == "Enumeration") {
            enumerationAttribute = make_unique<EnumerationAttribute>(EnumerationAttribute());
            enumerationAttribute->load(v.second);
        }
    }

}

IntegerVariable ScalarVariable::asIntegerVariable() {
    return IntegerVariable(*integerAttribute);
}

RealVariable ScalarVariable::asRealVariable() {
    return RealVariable(*realAttribute);
}

StringVariable ScalarVariable::asStringVariable() {
    return StringVariable(*stringAttribute);
}

BooleanVariable ScalarVariable::asBooleanVariable() {
    return BooleanVariable(*booleanAttribute);
}

EnumerationVariable ScalarVariable::asEnumerationVariable() {
    return EnumerationVariable(*enumerationAttribute);
}

IntegerVariable::IntegerVariable(const IntegerAttribute &attribute)
    : min(attribute.min), max(attribute.max), start(attribute.start) {};

RealVariable::RealVariable(const RealAttribute &attribute)
    : min(attribute.min), max(attribute.max), start(attribute.start) {};

StringVariable::StringVariable(const StringAttribute &attribute)
    : start(attribute.start) {};

BooleanVariable::BooleanVariable(const BooleanAttribute &attribute)
    : start(attribute.start) {};

EnumerationVariable::EnumerationVariable(const EnumerationAttribute &attribute)
    : min(attribute.min), max(attribute.max), start(attribute.start) {};

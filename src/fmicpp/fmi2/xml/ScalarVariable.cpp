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


void ScalarVariable::load(const ptree &node) {

    name_ = node.get<string>("<xmlattr>.name");
    description_ = node.get<string>("<xmlattr>.description", "");
    valueReference_ = node.get<fmi2ValueReference>("<xmlattr>.valueReference");
    canHandleMultipleSetPerTimelnstant_ = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

    causality_ = parseCausality(node.get<string>("<xmlattr>.causality", ""));
    variability_ = parseVariability(node.get<string>("<xmlattr>.variability", ""));
    initial_ = parseInitial(node.get<string>("<xmlattr>.initial", ""));

    for (const ptree::value_type &v : node) {
        if (v.first == "Integer") {
            integerAttribute_ = make_unique<IntegerAttribute>(IntegerAttribute());
            integerAttribute_->load(v.second);
        } else if (v.first == "Real") {
            realAttribute_ = make_unique<RealAttribute>(RealAttribute());
            realAttribute_->load(v.second);
        } else if (v.first == "String") {
            stringAttribute_ = make_unique<StringAttribute>(StringAttribute());
            stringAttribute_->load(v.second);
        } else if (v.first == "Boolean") {
            booleanAttribute_ = make_unique<BooleanAttribute>(BooleanAttribute());
            booleanAttribute_->load(v.second);
        } else if (v.first == "Enumeration") {
            enumerationAttribute_ = make_unique<EnumerationAttribute>(EnumerationAttribute());
            enumerationAttribute_->load(v.second);
        }
    }

}

IntegerVariable ScalarVariable::asIntegerVariable() {
    if (integerAttribute_ == nullptr) {
        throw runtime_error(getName() + "is not of type Integer!");
    }
    return IntegerVariable(*this, *integerAttribute_);
}

RealVariable ScalarVariable::asRealVariable() {
    if (realAttribute_ == nullptr) {
        throw runtime_error(getName() + "is not of type Real!");
    }
    return RealVariable(*this, *realAttribute_);
}

StringVariable ScalarVariable::asStringVariable() {
    if (stringAttribute_ == nullptr) {
        throw runtime_error(getName() + "is not of type String!");
    }
    return StringVariable(*this, *stringAttribute_);
}

BooleanVariable ScalarVariable::asBooleanVariable() {
    if (booleanAttribute_ == nullptr) {
        throw runtime_error(getName() + "is not of type Boolean!");
    }
    return BooleanVariable(*this, *booleanAttribute_);
}

EnumerationVariable ScalarVariable::asEnumerationVariable() {
    if (enumerationAttribute_ == nullptr) {
        throw runtime_error(getName() + "is not of type Enumeration!");
    }
    return EnumerationVariable(*this, *enumerationAttribute_);
}

string ScalarVariable::getName() const {
    return name_;
}

string ScalarVariable::getDescription() const {
    return description_;
}

fmi2ValueReference ScalarVariable::getValueReference() const {
    return valueReference_;
}

bool ScalarVariable::canHandleMultipleSetPerTimelnstant() const {
    return canHandleMultipleSetPerTimelnstant_;
}

fmi2Causality ScalarVariable::getCausality() const {
    return causality_;
}

fmi2Variability ScalarVariable::getVariability() const {
    return variability_;
}

fmi2Initial ScalarVariable::getInitial() const {
    return initial_;
}

IntegerVariable::IntegerVariable(const ScalarVariable &var, IntegerAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

shared_ptr<int> IntegerVariable::getMin() const {
    return attribute_.min;
}

shared_ptr<int> IntegerVariable::getMax() const {
    return attribute_.max;
}

shared_ptr<int> IntegerVariable::getStart() const {
    return attribute_.start;
}

void IntegerVariable::setStart(const int start) {
    attribute_.start = make_shared<int>(start);
}

shared_ptr<string> IntegerVariable::getQuantity() const {
    return attribute_.quantity;
}

RealVariable::RealVariable(const ScalarVariable &var, RealAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

shared_ptr<double> RealVariable::getMin() const {
    return attribute_.min;
}

shared_ptr<double> RealVariable::getMax() const {
    return attribute_.max;
}

shared_ptr<double> RealVariable::getStart() const {
    return attribute_.start;
}

void RealVariable::setStart(const double start) {
    attribute_.start = make_shared<double>(start);
}

shared_ptr<double> RealVariable::getNominal() const {
    return attribute_.nominal;
}

bool RealVariable::getReinit() const {
    return attribute_.reinit;
}

bool RealVariable::getUnbounded() const {
    return attribute_.unbounded;
}

bool RealVariable::getRelativeQuantity() const {
    return attribute_.relativeQuantity;
}

shared_ptr<string> RealVariable::getQuantity() const {
    return attribute_.quantity;
}

shared_ptr<string> RealVariable::getUnit() const {
    return attribute_.unit;
}

shared_ptr<string> RealVariable::getDisplayUnit() const {
    return attribute_.displayUnit;
}

shared_ptr<unsigned int> RealVariable::getDerivative() const {
    return attribute_.derivative;
}

StringVariable::StringVariable(const ScalarVariable &var, StringAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

shared_ptr<string> StringVariable::getStart() const {
    return attribute_.start;
}

void StringVariable::setStart(const string &start) {
    attribute_.start = make_shared<string>(start);
}

BooleanVariable::BooleanVariable(const ScalarVariable &var, BooleanAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

shared_ptr<bool> BooleanVariable::getStart() const {
    return attribute_.start;
}

void BooleanVariable::setStart(const bool start) {
    attribute_.start = make_shared<bool>(start);
}

EnumerationVariable::EnumerationVariable(const ScalarVariable &var, EnumerationAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

shared_ptr<int> EnumerationVariable::getMin() const {
    return attribute_.min;
}

shared_ptr<int> EnumerationVariable::getMax() const {
    return attribute_.max;
}

shared_ptr<int> EnumerationVariable::getStart() const {
    return attribute_.start;
}

void EnumerationVariable::setStart(const int start) {
    attribute_.start = make_shared<int>(start);
}

shared_ptr<string> EnumerationVariable::getQuantity() const {
    return attribute_.quantity;
}

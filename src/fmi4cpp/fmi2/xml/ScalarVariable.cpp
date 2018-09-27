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

#include <fmi4cpp/fmi2/enumsToString.hpp>
#include <fmi4cpp/fmi2/xml/ScalarVariable.hpp>

using namespace std;
using namespace fmi4cpp::fmi2::xml;

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
            integerAttribute_ = make_shared<IntegerAttribute>();
            integerAttribute_->load(v.second);
        } else if (v.first == "Real") {
            realAttribute_ = make_shared<RealAttribute>();
            realAttribute_->load(v.second);
        } else if (v.first == "String") {
            stringAttribute_ = make_shared<StringAttribute>();
            stringAttribute_->load(v.second);
        } else if (v.first == "Boolean") {
            booleanAttribute_ = make_shared<BooleanAttribute>();
            booleanAttribute_->load(v.second);
        } else if (v.first == "Enumeration") {
            enumerationAttribute_ = make_shared<EnumerationAttribute>();
            enumerationAttribute_->load(v.second);
        }
    }

}

IntegerVariable ScalarVariable::asIntegerVariable() const {
    if (!isIntegerVariable()) {
        throw runtime_error(getName() + "is not of type Integer!");
    }
    return IntegerVariable(*this, *integerAttribute_);
}

RealVariable ScalarVariable::asRealVariable() const {
    if (!isRealVariable()) {
        throw runtime_error(getName() + "is not of type Real!");
    }
    return RealVariable(*this, *realAttribute_);
}

StringVariable ScalarVariable::asStringVariable() const {
    if (!isStringVariable()) {
        throw runtime_error(getName() + "is not of type String!");
    }
    return StringVariable(*this, *stringAttribute_);
}

BooleanVariable ScalarVariable::asBooleanVariable() const {
    if (!isBooleanVariable()) {
        throw runtime_error(getName() + "is not of type Boolean!");
    }
    return BooleanVariable(*this, *booleanAttribute_);
}

EnumerationVariable ScalarVariable::asEnumerationVariable() const {
    if (!isEnumerationVariable()) {
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

ostream &fmi4cpp::fmi2::xml::operator<<(ostream &os, const ScalarVariable &variable) {
    os << "ScalarVariable( name: " << variable.name_ << ", description: " << variable.description_ << ", valueReference: "
       << variable.valueReference_ << ", causality: " << to_string(variable.causality_) << ", variability: "
       <<  to_string(variable.variability_) << ", initial: " <<  to_string(variable.initial_);

    if (variable.integerAttribute_) {
        os << ", IntegerAttribute_(" << *variable.integerAttribute_ << " )";
    } else if (variable.realAttribute_) {
        os << ", RealAttribute(" << *variable.realAttribute_ << " )";
    }

    os << " )";

    return os;
}

bool ScalarVariable::isIntegerVariable() const {
    return integerAttribute_ != nullptr;
}

bool ScalarVariable::isRealVariable() const {
    return realAttribute_ != nullptr;
}

bool ScalarVariable::isStringVariable() const {
    return stringAttribute_ != nullptr;
}

bool ScalarVariable::isBooleanVariable() const {
    return booleanAttribute_ != nullptr;
}

bool ScalarVariable::isEnumerationVariable() const {
    return enumerationAttribute_ != nullptr;
}

IntegerVariable::IntegerVariable(const ScalarVariable &var, IntegerAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

std::optional<int> IntegerVariable::getMin() const {
    return attribute_.min;
}

std::optional<int> IntegerVariable::getMax() const {
    return attribute_.max;
}

std::optional<int> IntegerVariable::getStart() const {
    return attribute_.start;
}

void IntegerVariable::setStart(const int start) {
    attribute_.start = start;
}

std::optional<string> IntegerVariable::getQuantity() const {
    return attribute_.quantity;
}

RealVariable::RealVariable(const ScalarVariable &var, RealAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

std::optional<double> RealVariable::getMin() const {
    return attribute_.min;
}

std::optional<double> RealVariable::getMax() const {
    return attribute_.max;
}

std::optional<double> RealVariable::getStart() const {
    return attribute_.start;
}

void RealVariable::setStart(const double start) {
    attribute_.start = start;
}

std::optional<double> RealVariable::getNominal() const {
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

std::optional<string> RealVariable::getQuantity() const {
    return attribute_.quantity;
}

std::optional<string> RealVariable::getUnit() const {
    return attribute_.unit;
}

std::optional<string> RealVariable::getDisplayUnit() const {
    return attribute_.displayUnit;
}

std::optional<unsigned int> RealVariable::getDerivative() const {
    return attribute_.derivative;
}

StringVariable::StringVariable(const ScalarVariable &var, StringAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

std::optional<string> StringVariable::getStart() const {
    return attribute_.start;
}

void StringVariable::setStart(const string &start) {
    attribute_.start = start;
}

BooleanVariable::BooleanVariable(const ScalarVariable &var, BooleanAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

std::optional<bool> BooleanVariable::getStart() const {
    return attribute_.start;
}

void BooleanVariable::setStart(const bool start) {
    attribute_.start = start;
}

EnumerationVariable::EnumerationVariable(const ScalarVariable &var, EnumerationAttribute &attribute)
        : ScalarVariable(var), attribute_(attribute) {}

std::optional<int> EnumerationVariable::getMin() const {
    return attribute_.min;
}

std::optional<int> EnumerationVariable::getMax() const {
    return attribute_.max;
}

std::optional<int> EnumerationVariable::getStart() const {
    return attribute_.start;
}

void EnumerationVariable::setStart(const int start) {
    attribute_.start = start;
}

std::optional<string> EnumerationVariable::getQuantity() const {
    return attribute_.quantity;
}

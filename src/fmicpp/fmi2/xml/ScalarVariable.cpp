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

#include <boost/optional.hpp>
#include <fmicpp/fmi2/xml/ScalarVariable.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;

namespace {

    fmi2Causality parseCausality(string str) {
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

    fmi2Variability parseVariability(string str) {
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
            return fmi2Variability ::continuous;
        }
    }

    fmi2Initial parseInitial(string str) {
        if (str == "exact") {
            return fmi2Initial::exact;
        } else if (str == "approx") {
            return fmi2Initial::approx;
        } else if (str == "calculated") {
            return fmi2Initial ::calculated;
        } else {
            return fmi2Initial::unknown;
        }
    }

}

void ScalarVariable::load(const ptree &node) {

    name_ = node.get<string>("<xmlattr>.name");
    description_ = node.get<string>("<xmlattr>.description", "");
    valueReference_ = node.get<fmi2ValueReference >("<xmlattr>.valueReference");
    canHandleMultipleSetPerTimelnstant_ = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

    causality_ = parseCausality(node.get<string >("<xmlattr>.causality", ""));
    variability_ = parseVariability(node.get<string >("<xmlattr>.variability", ""));
    initial_ =  parseInitial(node.get<string>("<xmlattr>.initial", ""));

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
    return IntegerVariable(*integerAttribute_);
}

RealVariable ScalarVariable::asRealVariable() {
    return RealVariable(*realAttribute_);
}

StringVariable ScalarVariable::asStringVariable() {
    return StringVariable(*stringAttribute_);
}

BooleanVariable ScalarVariable::asBooleanVariable() {
    return BooleanVariable(*booleanAttribute_);
}

EnumerationVariable ScalarVariable::asEnumerationVariable() {
    return EnumerationVariable(*enumerationAttribute_);
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

IntegerVariable::IntegerVariable(const IntegerAttribute attribute): attribute_(attribute) {}

int IntegerVariable::getMin() const {
    return attribute_.min;
}

int IntegerVariable::getMax() const {
    return attribute_.max;
}

int IntegerVariable::getStart() const {
    return attribute_.start;
}

string IntegerVariable::getQuantity() const {
    return attribute_.quantity;
}

RealVariable::RealVariable(const RealAttribute attribute): attribute_(attribute) {}

double RealVariable::getMin() const {
    return attribute_.min;
}

double RealVariable::getMax() const {
    return attribute_.max;
}

double RealVariable::getStart() const {
    return attribute_.start;
}

void RealVariable::setStart(const double start) {
    attribute_.start = start;
}

double RealVariable::getNominal() const {
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

string RealVariable::getQuantity() const {
    return attribute_.quantity;
}

string RealVariable::getUnit() const {
    return attribute_.unit;
}

string RealVariable::getDisplayUnit() const {
    return attribute_.displayUnit;
}

unsigned int RealVariable::getDerivative() const {
    return attribute_.derivative;
}

StringVariable::StringVariable(const StringAttribute attribute): attribute_(attribute) {}

string StringVariable::getStart() const {
    return attribute_.start;
}

void StringVariable::setStart(string start) {
    attribute_.start = start;
}

BooleanVariable::BooleanVariable(const BooleanAttribute attribute): attribute_(attribute) {}

bool BooleanVariable::getStart() const {
    return attribute_.start;
}

void BooleanVariable::setStart(bool start) {
    attribute_.start = start;
}

EnumerationVariable::EnumerationVariable(const EnumerationAttribute attribute): attribute_(attribute) {}

int EnumerationVariable::getMin() const {
    return attribute_.min;
}

int EnumerationVariable::getMax() const {
    return attribute_.max;
}

int EnumerationVariable::getStart() const {
    return attribute_.start;
}

void EnumerationVariable::setStart(int start) {
    attribute_.start = start;
}

string EnumerationVariable::getQuantity() const {
    return attribute_.quantity;
}

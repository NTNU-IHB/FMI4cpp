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

#include <memory>
#include <sstream>
#include <stdexcept>
#include <fmi4cpp/fmi2/xml/enums.hpp>
#include <fmi4cpp/fmi2/enumsToString.hpp>
#include <fmi4cpp/fmi2/xml/ScalarVariableImpl.hpp>

using namespace fmi4cpp::fmi2::xml;

ScalarVariableImpl::ScalarVariableImpl(const ptree &node) {

    name_ = node.get<std::string>("<xmlattr>.name");
    description_ = node.get<std::string>("<xmlattr>.description", "");
    valueReference_ = node.get<fmi2ValueReference>("<xmlattr>.valueReference");
    canHandleMultipleSetPerTimelnstant_ = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

    causality_ = parseCausality(node.get<std::string>("<xmlattr>.causality", ""));
    variability_ = parseVariability(node.get<std::string>("<xmlattr>.variability", ""));
    initial_ = parseInitial(node.get<std::string>("<xmlattr>.initial", ""));

    for (const ptree::value_type &v : node) {
        if (v.first == "Integer") {
            integerAttribute_ = std::make_shared<IntegerAttribute>();
            integerAttribute_->load(v.second);
        } else if (v.first == "Real") {
            realAttribute_ = std::make_shared<RealAttribute>();
            realAttribute_->load(v.second);
        } else if (v.first == "String") {
            stringAttribute_ = std::make_shared<StringAttribute>();
            stringAttribute_->load(v.second);
        } else if (v.first == "Boolean") {
            booleanAttribute_ = std::make_shared<BooleanAttribute>();
            booleanAttribute_->load(v.second);
        } else if (v.first == "Enumeration") {
            enumerationAttribute_ = std::make_shared<EnumerationAttribute>();
            enumerationAttribute_->load(v.second);
        }
    }

}

IntegerVariable ScalarVariableImpl::asIntegerVariable() const {
    if (!isIntegerVariable()) {
        throw std::runtime_error(getName() + "is not of type Integer!");
    }
    return IntegerVariable(*this, *integerAttribute_);
}

RealVariable ScalarVariableImpl::asRealVariable() const {
    if (!isRealVariable()) {
        throw std::runtime_error(getName() + "is not of type Real!");
    }
    return RealVariable(*this, *realAttribute_);
}

StringVariable ScalarVariableImpl::asStringVariable() const {
    if (!isStringVariable()) {
        throw std::runtime_error(getName() + "is not of type String!");
    }
    return StringVariable(*this, *stringAttribute_);
}

BooleanVariable ScalarVariableImpl::asBooleanVariable() const {
    if (!isBooleanVariable()) {
        throw std::runtime_error(getName() + "is not of type Boolean!");
    }
    return BooleanVariable(*this, *booleanAttribute_);
}

EnumerationVariable ScalarVariableImpl::asEnumerationVariable() const {
    if (!isEnumerationVariable()) {
        throw  std::runtime_error(getName() + "is not of type Enumeration!");
    }
    return EnumerationVariable(*this, *enumerationAttribute_);
}

std::string ScalarVariableImpl::getName() const {
    return name_;
}

std::string ScalarVariableImpl::getDescription() const {
    return description_;
}

fmi2ValueReference ScalarVariableImpl::getValueReference() const {
    return valueReference_;
}

bool ScalarVariableImpl::canHandleMultipleSetPerTimelnstant() const {
    return canHandleMultipleSetPerTimelnstant_;
}

fmi2Causality ScalarVariableImpl::getCausality() const {
    return causality_;
}

fmi2Variability ScalarVariableImpl::getVariability() const {
    return variability_;
}

fmi2Initial ScalarVariableImpl::getInitial() const {
    return initial_;
}


bool ScalarVariableImpl::isIntegerVariable() const {
    return integerAttribute_ != nullptr;
}

bool ScalarVariableImpl::isRealVariable() const {
    return realAttribute_ != nullptr;
}

bool ScalarVariableImpl::isStringVariable() const {
    return stringAttribute_ != nullptr;
}

bool ScalarVariableImpl::isBooleanVariable() const {
    return booleanAttribute_ != nullptr;
}

bool ScalarVariableImpl::isEnumerationVariable() const {
    return enumerationAttribute_ != nullptr;
}

std::string fmi4cpp::fmi2::xml::ScalarVariableImpl::toString() const {

    std::ostringstream os;

    os << "ScalarVariable( name=" << name_ << ", description=" << description_ << ", valueReference="
       << valueReference_ << ", causality=" << to_string(causality_) << ", variability="
       <<  to_string(variability_) << ", initial=" <<  to_string(initial_);

    if (integerAttribute_) {
        os << ", IntegerAttribute_(" << *integerAttribute_ << " )";
    } else if (realAttribute_) {
        os << ", RealAttribute(" << *realAttribute_ << " )";
    } else if (stringAttribute_) {
        os << ", StringAttribute(" << *stringAttribute_ << " )";
    } else if (booleanAttribute_) {
        os << ", BooleanAttribute(" << *booleanAttribute_ << " )";
    } else if (enumerationAttribute_) {
        os << ", EnumerationAttribute(" << *enumerationAttribute_ << " )";
    }

    os << " )";

    return os.str();

}


IntegerVariable::IntegerVariable(const ScalarVariableImpl &var, const IntegerAttribute &attribute)
        : ScalarVariableImpl(var), BoundedVariable<int, IntegerAttribute>(attribute) {}


RealVariable::RealVariable(const ScalarVariableImpl &var, const RealAttribute &attribute)
        : ScalarVariableImpl(var), BoundedVariable<double, RealAttribute>(attribute) {}


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

std::optional<std::string> RealVariable::getUnit() const {
    return attribute_.unit;
}

std::optional<std::string> RealVariable::getDisplayUnit() const {
    return attribute_.displayUnit;
}

std::optional<unsigned int> RealVariable::getDerivative() const {
    return attribute_.derivative;
}

StringVariable::StringVariable(const ScalarVariableImpl &var, const StringAttribute &attribute)
        : ScalarVariableImpl(var), TypedVariable<std::string, StringAttribute>(attribute) {}

BooleanVariable::BooleanVariable(const ScalarVariableImpl &var, const BooleanAttribute &attribute)
        : ScalarVariableImpl(var), TypedVariable<bool, BooleanAttribute>(attribute) {}


EnumerationVariable::EnumerationVariable(const ScalarVariableImpl &var, const EnumerationAttribute &attribute)
        : ScalarVariableImpl(var), BoundedVariable<int, EnumerationAttribute>(attribute) {}


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

#include <fmi4cpp/fmi2/xml/ScalarVariable.hpp>
#include <fmi4cpp/fmi2/xml/TypedScalarVariable.hpp>

using namespace fmi4cpp::fmi2;

std::string ScalarVariableBase::name() const {
    return name_;
}

std::string ScalarVariableBase::description() const {
    return description_;
}

fmi2ValueReference ScalarVariableBase::valueReference() const {
    return valueReference_;
}

fmi2Causality ScalarVariableBase::causality() const {
    return causality_;
}

fmi2Variability ScalarVariableBase::variability() const {
    return variability_;
}

fmi2Initial ScalarVariableBase::initial() const {
    return initial_;
}

bool ScalarVariableBase::canHandleMultipleSetPerTimelnstant() const {
    return canHandleMultipleSetPerTimelnstant_;
}

ScalarVariableBase::ScalarVariableBase(const std::string &name,
                                       const std::string &description,
                                       fmi2ValueReference valueReference,
                                       bool canHandleMultipleSetPerTimelnstant,
                                       fmi2Causality causality,
                                       fmi2Variability variability,
                                       fmi2Initial initial)
        : name_(name),
          description_(description),
          valueReference_(valueReference),
          canHandleMultipleSetPerTimelnstant_(canHandleMultipleSetPerTimelnstant),
          causality_(causality),
          variability_(variability),
          initial_(initial) {}

ScalarVariable::ScalarVariable(const ScalarVariableBase &base,
                               const IntegerAttribute &integer)
        : ScalarVariableBase(base), integer_(integer) {}

ScalarVariable::ScalarVariable(const ScalarVariableBase &base,
                               const RealAttribute &real)
        : ScalarVariableBase(base), real_(real) {}

ScalarVariable::ScalarVariable(const ScalarVariableBase &base,
                               const StringAttribute &string)
        : ScalarVariableBase(base), string_(string) {}

ScalarVariable::ScalarVariable(const ScalarVariableBase &base,
                               const BooleanAttribute &boolean)
        : ScalarVariableBase(base), boolean_(boolean) {}

ScalarVariable::ScalarVariable(const ScalarVariableBase &base,
                               const EnumerationAttribute &enumeration)
        : ScalarVariableBase(base), enumeration_(enumeration) {}


bool ScalarVariable::isInteger() const {
    return integer_.has_value();
}

bool ScalarVariable::isReal() const {
    return real_.has_value();
}

bool ScalarVariable::isString() const {
    return string_.has_value();
}

bool ScalarVariable::isBoolean() const {
    return boolean_.has_value();
}

bool ScalarVariable::isEnumeration() const {
    return enumeration_.has_value();
}

const IntegerVariable ScalarVariable::asInteger() const {
    return IntegerVariable(*this, *integer_);
}

const RealVariable ScalarVariable::asReal() const {
    return RealVariable(*this, *real_);
}

const StringVariable ScalarVariable::asString() const {
    return StringVariable(*this, *string_);
}

const BooleanVariable ScalarVariable::asBoolean() const {
    return BooleanVariable(*this, *boolean_);
}

const EnumerationVariable ScalarVariable::asEnumeration() const {
    return EnumerationVariable(*this, *enumeration_);
}

std::string ScalarVariable::typeName() const {
    if (integer_) {
        return INTEGER_TYPE;
    } else if (real_) {
        return REAL_TYPE;
    } else if (string_) {
        return STRING_TYPE;
    } else if (boolean_) {
        return BOOLEAN_TYPE;
    } else if (enumeration_) {
        return ENUMERATION_TYPE;
    } else {
        return "Unknown";
    }
}

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

#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>
#include <fmi4cpp/fmi2/xml/typed_scalar_variable.hpp>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;


scalar_variable::scalar_variable(const scalar_variable_base &base,
                               const integer_attribute &integer)
        : scalar_variable_base(base), integer_(integer) {}

scalar_variable::scalar_variable(const scalar_variable_base &base,
                               const real_attribute &real)
        : scalar_variable_base(base), real_(real) {}

scalar_variable::scalar_variable(const scalar_variable_base &base,
                               const string_attribute &string)
        : scalar_variable_base(base), string_(string) {}

scalar_variable::scalar_variable(const scalar_variable_base &base,
                               const boolean_attribute &boolean)
        : scalar_variable_base(base), boolean_(boolean) {}

scalar_variable::scalar_variable(const scalar_variable_base &base,
                               const enumeration_attribute &enumeration)
        : scalar_variable_base(base), enumeration_(enumeration) {}


bool scalar_variable::isInteger() const {
    return integer_.has_value();
}

bool scalar_variable::isReal() const {
    return real_.has_value();
}

bool scalar_variable::isString() const {
    return string_.has_value();
}

bool scalar_variable::isBoolean() const {
    return boolean_.has_value();
}

bool scalar_variable::isEnumeration() const {
    return enumeration_.has_value();
}

const IntegerVariable scalar_variable::asInteger() const {
    return IntegerVariable(*this, *integer_);
}

const RealVariable scalar_variable::asReal() const {
    return RealVariable(*this, *real_);
}

const StringVariable scalar_variable::asString() const {
    return StringVariable(*this, *string_);
}

const BooleanVariable scalar_variable::asBoolean() const {
    return BooleanVariable(*this, *boolean_);
}

const EnumerationVariable scalar_variable::asEnumeration() const {
    return EnumerationVariable(*this, *enumeration_);
}

std::string scalar_variable::typeName() const {
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
        return UNKNOWN_TYPE;
    }
}


IntegerVariable::IntegerVariable(const scalar_variable &variable, const integer_attribute &attribute)
        : BoundedScalarVariable(variable, attribute) {}

bool IntegerVariable::read(fmu_reader &reader, int &ref) {
    return reader.readInteger(valueReference(), ref);
}

bool IntegerVariable::write(fmu_writer &writer, int value) {
    return writer.writeInteger(valueReference(), value);
}

RealVariable::RealVariable(const scalar_variable &variable, const real_attribute &attribute)
        : BoundedScalarVariable(variable, attribute) {}

std::optional<std::string> RealVariable::displayUnit() const {
    return attribute_.displayUnit;
}

std::optional<std::string> RealVariable::unit() const {
    return attribute_.unit;
}

std::optional<size_t> RealVariable::derivative() const {
    return attribute_.derivative;
}

std::optional<double> RealVariable::nominal() const {
    return attribute_.nominal;
}

bool RealVariable::relativeQuantity() const {
    return attribute_.relativeQuantity;
}

bool RealVariable::unbounded() const {
    return attribute_.unbounded;
}

bool RealVariable::reinit() const {
    return attribute_.reinit;
}

bool RealVariable::read(fmu_reader &reader, double &ref) {
    return reader.readReal(valueReference(), ref);
}

bool RealVariable::write(fmu_writer &writer, double value) {
    return writer.writeReal(valueReference(), value);
}


StringVariable::StringVariable(const scalar_variable &variable, const string_attribute &attribute) : typed_scalar_variable(
        variable, attribute) {}

bool StringVariable::read(fmu_reader &reader, std::string &ref) {
    fmi2String str;
    auto status = reader.readString(valueReference(), str);
    ref = str;
    return status;
}

bool StringVariable::write(fmu_writer &writer, std::string value) {
    return writer.writeString(valueReference(), value.c_str());
}


BooleanVariable::BooleanVariable(const scalar_variable &variable, const boolean_attribute &attribute)
        : typed_scalar_variable(variable, attribute) {}

bool BooleanVariable::read(fmu_reader &reader, bool &ref) {
    fmi2Boolean _ref;
    auto status = reader.readBoolean(valueReference(), _ref);
    ref = _ref != 0;
    return status;
}

bool BooleanVariable::write(fmu_writer &writer, bool value) {
    return writer.writeBoolean(valueReference(), value);
}


EnumerationVariable::EnumerationVariable(const scalar_variable &variable, const enumeration_attribute &attribute)
        : typed_scalar_variable(variable, attribute) {}

bool EnumerationVariable::read(fmu_reader &reader, int &ref) {
    return reader.readInteger(valueReference(), ref);
}

bool EnumerationVariable::write(fmu_writer &writer, int value) {
    return writer.writeInteger(valueReference(), value);
}




integer_attribute::integer_attribute(const bounded_scalar_variable_attribute<int> &attributes)
        : bounded_scalar_variable_attribute<int>(attributes) {}


real_attribute::real_attribute(const bounded_scalar_variable_attribute<double> &attributes)
        : bounded_scalar_variable_attribute<double>(attributes) {}


string_attribute::string_attribute(const scalar_variable_attribute<std::string> &attributes)
        : scalar_variable_attribute<std::string>(attributes) {}


boolean_attribute::boolean_attribute(const scalar_variable_attribute<bool> &attributes)
        : scalar_variable_attribute<bool>(attributes) {}


enumeration_attribute::enumeration_attribute(const bounded_scalar_variable_attribute<int> &attributes)
        : bounded_scalar_variable_attribute<int>(attributes) {}

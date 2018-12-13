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

#include <fmi4cpp/fmi2/xml/TypedScalarVariable.hpp>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

IntegerVariable::IntegerVariable(const ScalarVariable &variable, const IntegerAttribute &attribute)
        : BoundedScalarVariable(variable, attribute) {}

bool IntegerVariable::read(FmuReader &reader, int &ref) {
    return reader.readInteger(valueReference(), ref);
}

bool IntegerVariable::write(FmuWriter &writer, int value) {
    return writer.writeInteger(valueReference(), value);
}

RealVariable::RealVariable(const ScalarVariable &variable, const RealAttribute &attribute)
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

bool RealVariable::read(FmuReader &reader, double &ref) {
    return reader.readReal(valueReference(), ref);
}

bool RealVariable::write(FmuWriter &writer, double value) {
    return writer.writeReal(valueReference(), value);
}


StringVariable::StringVariable(const ScalarVariable &variable, const StringAttribute &attribute) : TypedScalarVariable(
        variable, attribute) {}

bool StringVariable::read(FmuReader &reader, std::string &ref) {
    fmi2String str;
    auto status = reader.readString(valueReference(), str);
    ref = str;
    return status;
}

bool StringVariable::write(FmuWriter &writer, std::string value) {
    return writer.writeString(valueReference(), value.c_str());
}


BooleanVariable::BooleanVariable(const ScalarVariable &variable, const BooleanAttribute &attribute)
        : TypedScalarVariable(variable, attribute) {}

bool BooleanVariable::read(FmuReader &reader, bool &ref) {
    fmi2Boolean _ref;
    auto status = reader.readBoolean(valueReference(), _ref);
    ref = _ref != 0;
    return status;
}

bool BooleanVariable::write(FmuWriter &writer, bool value) {
    return writer.writeBoolean(valueReference(), value);
}


EnumerationVariable::EnumerationVariable(const ScalarVariable &variable, const EnumerationAttribute &attribute)
        : TypedScalarVariable(variable, attribute) {}

bool EnumerationVariable::read(FmuReader &reader, int &ref) {
    return reader.readInteger(valueReference(), ref);
}

bool EnumerationVariable::write(FmuWriter &writer, int value) {
    return writer.writeInteger(valueReference(), value);
}


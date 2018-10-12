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

#include <fmi4cpp/fmi2/xml/ScalarVariableAttributes.hpp>

using namespace fmi4cpp::fmi2::xml;


IntegerAttribute::IntegerAttribute(const BoundedScalarVariableAttributes<int> &attributes)
        : BoundedScalarVariableAttributes<int>(attributes) {}


RealAttribute::RealAttribute(const BoundedScalarVariableAttributes<double> &attributes,
                             const bool reinit, const bool unbounded, const bool relativeQuantity,
                             const std::optional<double> &nominal, const std::optional<size_t> &derivative,
                             const std::optional<std::string> &unit, const std::optional<std::string> &displayUnit)
        : BoundedScalarVariableAttributes<double>(attributes), reinit(reinit), unbounded(unbounded),
          relativeQuantity(relativeQuantity), nominal(nominal), derivative(derivative), unit(unit),
          displayUnit(displayUnit) {}


StringAttribute::StringAttribute(const ScalarVariableAttributes<std::string> &attributes)
        : ScalarVariableAttributes<std::string>(attributes) {}


BooleanAttribute::BooleanAttribute(const ScalarVariableAttributes<bool> &attributes)
        : ScalarVariableAttributes<bool>(attributes) {}


EnumerationAttribute::EnumerationAttribute(const ScalarVariableAttributes<int> &attributes)
        : ScalarVariableAttributes<int>(attributes) {}


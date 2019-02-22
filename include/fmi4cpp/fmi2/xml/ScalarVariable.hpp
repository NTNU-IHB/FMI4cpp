#include <utility>

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

#ifndef FMI4CPP_SCALARVARIABLE_HPP
#define FMI4CPP_SCALARVARIABLE_HPP

#include <optional>

#include "enums.hpp"
#include "../fmi2Functions.h"
#include "ScalarVariableAttribute.hpp"

namespace fmi4cpp::fmi2 {

    class IntegerVariable;

    class RealVariable;

    class StringVariable;

    class BooleanVariable;

    class EnumerationVariable;

    const std::string INTEGER_TYPE = "Integer";
    const std::string REAL_TYPE = "Real";
    const std::string STRING_TYPE = "String";
    const std::string BOOLEAN_TYPE = "Boolean";
    const std::string ENUMERATION_TYPE = "Enumeration";
    const std::string UNKNOWN_TYPE = "Unknown";

    struct ScalarVariableBase {

        std::string name;
        std::string description;

        Initial initial;
        Causality causality;
        Variability variability;

        fmi2ValueReference valueReference;
        bool canHandleMultipleSetPerTimelnstant;

    };

    class ScalarVariable : public ScalarVariableBase {

    private:

        std::optional<IntegerAttribute> integer_;
        std::optional<RealAttribute> real_;
        std::optional<StringAttribute> string_;
        std::optional<BooleanAttribute> boolean_;
        std::optional<EnumerationAttribute> enumeration_;

    public:

        ScalarVariable(const ScalarVariableBase &base,
                       const IntegerAttribute &integer);

        ScalarVariable(const ScalarVariableBase &base,
                       const RealAttribute &real);

        ScalarVariable(const ScalarVariableBase &base,
                       const StringAttribute &string);

        ScalarVariable(const ScalarVariableBase &base,
                       const BooleanAttribute &enumeration);

        ScalarVariable(const ScalarVariableBase &base,
                       const EnumerationAttribute &enumeration);


        bool isInteger() const;

        bool isReal() const;

        bool isString() const;

        bool isBoolean() const;

        bool isEnumeration() const;

        std::string typeName() const;

        const IntegerVariable asInteger() const;

        const RealVariable asReal() const;

        const StringVariable asString() const;

        const BooleanVariable asBoolean() const;

        const EnumerationVariable asEnumeration() const;

    };

}

#endif //FMI4CPP_SCALARVARIABLE_HPP

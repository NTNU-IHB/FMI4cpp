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

#include <memory>
#include <optional>

#include "enums.hpp"
#include "../fmi2Functions.h"
#include "ScalarVariableAttributes.hpp"

namespace fmi4cpp::fmi2::xml {

    const std::string INTEGER_TYPE = "Integer";
    const std::string REAL_TYPE = "Real";
    const std::string STRING_TYPE = "String";
    const std::string BOOLEAN_TYPE = "Boolean";
    const std::string ENUMERATION_TYPE = "Enumeration";

    class ScalarVariable {

    private:
        std::string name_;
        std::string description_;

        fmi2ValueReference valueReference_;

        fmi2Causality causality_;
        fmi2Variability variability_;
        fmi2Initial initial_;

        bool canHandleMultipleSetPerTimelnstant_;

        std::optional<IntegerAttribute> integer_;
        std::optional<RealAttribute> real_;
        std::optional<StringAttribute> string_;
        std::optional<BooleanAttribute> boolean_;
        std::optional< EnumerationAttribute> enumeration_;

    public:

        ScalarVariable(const std::string &name, const std::string &description, const fmi2ValueReference valueReference,
                       const bool canHandleMultipleSetPerTimelnstant, const fmi2Causality causality,
                       const fmi2Variability variability, const fmi2Initial initial,
                       const IntegerAttribute &integer);

        ScalarVariable(const std::string &name, const std::string &description, const fmi2ValueReference valueReference,
                       const bool canHandleMultipleSetPerTimelnstant, const fmi2Causality causality,
                       const fmi2Variability variability, const fmi2Initial initial,
                       const RealAttribute &real);

        ScalarVariable(const std::string &name, const std::string &description, const fmi2ValueReference valueReference,
                       const bool canHandleMultipleSetPerTimelnstant, const fmi2Causality causality,
                       const fmi2Variability variability, const fmi2Initial initial,
                       const StringAttribute &string);

        ScalarVariable(const std::string &name, const std::string &description, const fmi2ValueReference valueReference,
                       const bool canHandleMultipleSetPerTimelnstant, const fmi2Causality causality,
                       const fmi2Variability variability, const fmi2Initial initial,
                       const BooleanAttribute &enumeration);

        ScalarVariable(const std::string &name, const std::string &description, const fmi2ValueReference valueReference,
                       const bool canHandleMultipleSetPerTimelnstant, const fmi2Causality causality,
                       const fmi2Variability variability, const fmi2Initial initial,
                       const EnumerationAttribute &enumeration);

        std::string name() const;
        std::string description() const;

        fmi2ValueReference valueReference() const;

        fmi2Causality causality() const;
        fmi2Variability variability() const;
        fmi2Initial initial() const;

        bool canHandleMultipleSetPerTimelnstant() const;

        std::optional<IntegerAttribute> integer() const;
        std::optional<RealAttribute> real() const;
        std::optional<StringAttribute> string() const;
        std::optional<BooleanAttribute> boolean() const;
        std::optional< EnumerationAttribute> enumeration() const;

        std::string typeName() const;

    };


}

#endif //FMI4CPP_SCALARVARIABLE_HPP

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
#include <ostream>
#include <optional>

#include "../fmi2Functions.h"

#include "enums.hpp"
#include "ScalarVariableAttributes.hpp"

namespace fmi4cpp::fmi2::xml {

    //forward declarations
//    struct IntegerVariable;
//
//    struct RealVariable;
//
//    struct StringVariable;
//
//    struct BooleanVariable;
//
//    struct EnumerationVariable;

    struct ScalarVariable {

        const std::string name;

        const std::string description;

        const fmi2ValueReference valueReference;

        const bool canHandleMultipleSetPerTimelnstant;

        const fmi2Causality causality;

        const fmi2Variability variability;

        const fmi2Initial initial;

//        IntegerVariable asIntegerVariable() const = 0;
//
//        RealVariable asRealVariable() const = 0;
//
//        StringVariable asStringVariable() const = 0;
//
//        BooleanVariable asBooleanVariable() const = 0;
//
//        EnumerationVariable asEnumerationVariable() const = 0;

        bool isIntegerVariable() const;

        bool isRealVariable() const;

        bool isStringVariable() const;

        bool isBooleanVariable() const;

        bool isEnumerationVariable() const;

        virtual std::string toString() const;

        friend std::ostream &operator<<(std::ostream &os, const ScalarVariable &variable) {
            return os << variable.toString();
        }

    };

}

#endif //FMI4CPP_SCALARVARIABLE_HPP

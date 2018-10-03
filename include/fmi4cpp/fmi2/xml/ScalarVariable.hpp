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
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

#include "../fmi2Functions.h"

#include "enums.hpp"
#include "ScalarVariableAttributes.hpp"

using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    //forward declarations
    class IntegerVariable;

    class RealVariable;

    class StringVariable;

    class BooleanVariable;

    class EnumerationVariable;

    class ScalarVariable {

    public:

        virtual std::string getName() const = 0;

        virtual std::string getDescription() const = 0;

        virtual fmi2ValueReference getValueReference() const = 0;

        virtual bool canHandleMultipleSetPerTimelnstant() const = 0;

        virtual fmi2Causality getCausality() const = 0;

        virtual fmi2Variability getVariability() const = 0;

        virtual fmi2Initial getInitial() const = 0;

        virtual IntegerVariable asIntegerVariable() const = 0;

        virtual RealVariable asRealVariable() const = 0;

        virtual StringVariable asStringVariable() const = 0;

        virtual BooleanVariable asBooleanVariable() const = 0;

        virtual EnumerationVariable asEnumerationVariable() const = 0;

        virtual bool isIntegerVariable() const = 0;

        virtual bool isRealVariable() const = 0;

        virtual bool isStringVariable() const = 0;

        virtual bool isBooleanVariable() const = 0;

        virtual bool isEnumerationVariable() const = 0;

        virtual std::string toString() const = 0;

        friend std::ostream &operator<<(std::ostream &os, const ScalarVariable &variable) {
            os << variable.toString();
            return os;
        }

    };

//    typedef std::shared_ptr<ScalarVariable> ScalarVariableRef;

}

#endif //FMI4CPP_SCALARVARIABLE_HPP

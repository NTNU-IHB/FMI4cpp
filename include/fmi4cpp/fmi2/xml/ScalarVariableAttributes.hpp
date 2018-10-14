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

#ifndef FMI4CPP_SCALARVARIBALEATTRIBUTES_HPP
#define FMI4CPP_SCALARVARIBALEATTRIBUTES_HPP

#include <string>
#include <ostream>
#include <optional>

namespace fmi4cpp::fmi2::xml {

    template<typename T>
    struct ScalarVariableAttributes {

        std::optional<T> start;
        std::optional<std::string> declaredType;

    };


    template<typename T>
    struct BoundedScalarVariableAttributes : ScalarVariableAttributes<T> {

        std::optional<T> min;
        std::optional<T> max;
        std::optional<std::string> quantity;

        explicit BoundedScalarVariableAttributes(const ScalarVariableAttributes<T> &attributes)
                : ScalarVariableAttributes(attributes) {}

    };

    struct IntegerAttribute : BoundedScalarVariableAttributes<int> {

        IntegerAttribute(const BoundedScalarVariableAttributes<int> &attributes);

    };

    struct RealAttribute : BoundedScalarVariableAttributes<double> {

        bool reinit;
        bool unbounded;
        bool relativeQuantity;

        std::optional<double> nominal;
        std::optional<size_t> derivative;

        std::optional<std::string> unit;
        std::optional<std::string> displayUnit;

        RealAttribute(const BoundedScalarVariableAttributes<double> &attributes);

    };

    struct StringAttribute : ScalarVariableAttributes<std::string> {

        StringAttribute(const ScalarVariableAttributes<std::string> &attributes);

    };

    struct BooleanAttribute : ScalarVariableAttributes<bool> {

        BooleanAttribute(const ScalarVariableAttributes<bool> &attributes);

    };

    struct EnumerationAttribute : ScalarVariableAttributes<int> {

        EnumerationAttribute(const ScalarVariableAttributes<int> &attributes);

    };

}


#endif //FMI4CPP_SCALARVARIBALEATTRIBUTES_HPP

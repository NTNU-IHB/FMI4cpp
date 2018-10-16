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

namespace fmi4cpp::fmi2 {

    template<typename T>
    struct ScalarVariableAttribute {

        std::optional<T> start;
        std::optional<std::string> declaredType;

        ScalarVariableAttribute() {}

        explicit ScalarVariableAttribute(const std::optional<T> &start) : start(start) {}

        ScalarVariableAttribute(const std::optional<T> &start, const std::optional<std::string> &declaredType)
                : start(start), declaredType(declaredType) {}

    };


    template<typename T>
    struct BoundedScalarVariableAttribute : ScalarVariableAttribute<T> {

        std::optional<T> min;
        std::optional<T> max;
        std::optional<std::string> quantity;

        explicit BoundedScalarVariableAttribute(const ScalarVariableAttribute<T> &attributes)
                : ScalarVariableAttribute<T>(attributes) {}

        BoundedScalarVariableAttribute(const ScalarVariableAttribute<T> &attributes, const std::optional<T> &min,
                                       const std::optional<T> &max, const std::optional<std::string> &quantity)
                : min(min), max(max), quantity(quantity) {}

    };

    struct IntegerAttribute : BoundedScalarVariableAttribute<int> {

        IntegerAttribute(const BoundedScalarVariableAttribute<int> &attributes);

    };

    struct RealAttribute : BoundedScalarVariableAttribute<double> {

        bool reinit;
        bool unbounded;
        bool relativeQuantity;

        std::optional<double> nominal;
        std::optional<size_t> derivative;

        std::optional<std::string> unit;
        std::optional<std::string> displayUnit;

        RealAttribute(const BoundedScalarVariableAttribute<double> &attributes);

    };

    struct StringAttribute : ScalarVariableAttribute<std::string> {

        StringAttribute(const ScalarVariableAttribute<std::string> &attributes);

    };

    struct BooleanAttribute : ScalarVariableAttribute<bool> {

        BooleanAttribute(const ScalarVariableAttribute<bool> &attributes);

    };

    struct EnumerationAttribute : ScalarVariableAttribute<int> {

        EnumerationAttribute(const ScalarVariableAttribute<int> &attributes);

    };

}


#endif //FMI4CPP_SCALARVARIBALEATTRIBUTES_HPP

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
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    template <typename T>
    struct ScalarVariableAttribute {

        const std::optional<T> start;
        const std::optional<std::string> declaredType;

        ScalarVariableAttribute(const std::optional<T> &start, const std::optional<std::string> &declaredType) : start(
                start), declaredType(declaredType) {}

    };

    template <typename T>
    struct BoundedScalarVariableAttribute: ScalarVariableAttribute<T> {

        const std::optional<T> min;
        const std::optional<T> max;
        const std::optional<std::string> quantity;

        BoundedScalarVariableAttribute(const std::optional<T> &start, const std::optional<std::string> &declaredType,
                                       const std::optional<T> &min, const std::optional<T> &max,
                                       const std::optional<std::string> &quantity) : ScalarVariableAttribute(start,
                                                                                                             declaredType),
                                                                                     min(min), max(max),
                                                                                     quantity(quantity) {}


    };

    struct IntegerAttribute: BoundedScalarVariableAttribute<int> {

        IntegerAttribute(const std::optional<int> &start, const std::optional<std::string> &declaredType,
                         const std::optional<int> &min, const std::optional<int> &max,
                         const std::optional<std::string> &quantity);

        friend std::ostream &operator<<(std::ostream &os, const IntegerAttribute &attribute);

    };

    struct RealAttribute: BoundedScalarVariableAttribute<double> {

        const bool reinit;
        const bool unbounded;
        const bool relativeQuantity;

        const std::optional<double> nominal;
        const std::optional<size_t > derivative;

        const std::optional<std::string> unit;
        const std::optional<std::string> displayUnit;

        RealAttribute(const std::optional<double> &start, const std::optional<std::string> &declaredType,
                      const std::optional<double> &min, const std::optional<double> &max,
                      const std::optional<std::string> &quantity, const bool reinit, const bool unbounded,
                      const bool relativeQuantity, const std::optional<double> &nominal,
                      const std::optional<size_t> &derivative, const std::optional<std::string> &unit,
                      const std::optional<std::string> &displayUnit);

        friend std::ostream &operator<<(std::ostream &os, const RealAttribute &attribute);

    };

    struct StringAttribute: ScalarVariableAttribute<std::string> {

        StringAttribute(const std::optional<std::string> &start, const std::optional<std::string> &declaredType);

        friend std::ostream &operator<<(std::ostream &os, const StringAttribute &attribute);

    };

    struct BooleanAttribute: ScalarVariableAttribute<bool> {

        BooleanAttribute(const std::optional<bool> &start, const std::optional<std::string> &declaredType);

        friend std::ostream &operator<<(std::ostream &os, const BooleanAttribute &attribute);

    };

    struct EnumerationAttribute: BoundedScalarVariableAttribute<int> {

        EnumerationAttribute(const std::optional<int> &start, const std::optional<std::string> &declaredType,
                             const std::optional<int> &min, const std::optional<int> &max,
                             const std::optional<std::string> &quantity);

        friend std::ostream &operator<<(std::ostream &os, const EnumerationAttribute &attribute);

    };

}


#endif //FMI4CPP_SCALARVARIBALEATTRIBUTES_HPP

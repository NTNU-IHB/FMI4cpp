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

        std::optional<T> start;
        std::optional<std::string> declaredType;

        virtual void load(const ptree &node);

    };

    template <typename T>
    struct BoundedScalarVariableAttribute: ScalarVariableAttribute<T> {

        std::optional<T> min;
        std::optional<T> max;
        std::optional<std::string> quantity;

        void load(const ptree &node) override;

    };

    struct IntegerAttribute: BoundedScalarVariableAttribute<int> {

        void load(const ptree &node) override;

        friend std::ostream &operator<<(std::ostream &os, const IntegerAttribute &attribute);

    };

    struct RealAttribute: BoundedScalarVariableAttribute<double> {

        bool reinit;
        bool unbounded;
        bool relativeQuantity;

        std::optional<double> nominal;
        std::optional<size_t > derivative;

        std::optional<std::string> unit;
        std::optional<std::string> displayUnit;

        void load(const ptree &node) override;

        friend std::ostream &operator<<(std::ostream &os, const RealAttribute &attribute);

    };

    struct StringAttribute: ScalarVariableAttribute<std::string> {

        void load(const ptree &node);

        friend std::ostream &operator<<(std::ostream &os, const StringAttribute &attribute);

    };

    struct BooleanAttribute: ScalarVariableAttribute<bool> {

        void load(const ptree &node) override;

        friend std::ostream &operator<<(std::ostream &os, const BooleanAttribute &attribute);

    };

    struct EnumerationAttribute: BoundedScalarVariableAttribute<int> {

        void load(const ptree &node) override;

        friend std::ostream &operator<<(std::ostream &os, const EnumerationAttribute &attribute);

    };

}


#endif //FMI4CPP_SCALARVARIBALEATTRIBUTES_HPP

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

#include <optional>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <ostream>

using std::string;
using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    struct IntegerAttribute {

        std::optional<int> min;
        std::optional<int> max;
        std::optional<int> start;

        std::optional<string> quantity;

        void load(const ptree &node);

        friend std::ostream &operator<<(std::ostream &os, const IntegerAttribute &attribute);

    };

    struct RealAttribute {
        std::optional<double> min;
        std::optional<double> max;
        std::optional<double> start;
        std::optional<double> nominal;

        std::optional<string> quantity;
        std::optional<string> unit;
        std::optional<string> displayUnit;

        std::optional<unsigned int> derivative;

        bool reinit;
        bool unbounded;
        bool relativeQuantity;

        void load(const ptree &node);

        friend std::ostream &operator<<(std::ostream &os, const RealAttribute &attribute);

    };

    struct StringAttribute {
        std::optional<string> start;

        void load(const ptree &node);

        friend std::ostream &operator<<(std::ostream &os, const StringAttribute &attribute);

    };

    struct BooleanAttribute {
        std::optional<bool> start;

        void load(const ptree &node);

        friend std::ostream &operator<<(std::ostream &os, const BooleanAttribute &attribute);

    };

    struct EnumerationAttribute {
        std::optional<int> min;
        std::optional<int> max;
        std::optional<int> start;

        std::optional<string> quantity;

        void load(const ptree &node);

        friend std::ostream &operator<<(std::ostream &os, const EnumerationAttribute &attribute);

    };

}


#endif //FMI4CPP_SCALARVARIBALEATTRIBUTES_HPP

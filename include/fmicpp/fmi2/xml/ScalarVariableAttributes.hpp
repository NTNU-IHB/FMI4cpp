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

#ifndef FMICPP_SCALARVARIBALEATTRIBUTES_HPP
#define FMICPP_SCALARVARIBALEATTRIBUTES_HPP

#include <memory>
#include <boost/property_tree/ptree.hpp>

using std::string;
using std::shared_ptr;
using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {

    struct IntegerAttribute {

        shared_ptr<int> min;
        shared_ptr<int> max;
        shared_ptr<int> start;

        shared_ptr<string> quantity;

        void load(const ptree &node);
    };

    struct RealAttribute {
        shared_ptr<double> min;
        shared_ptr<double> max;
        shared_ptr<double> start;
        shared_ptr<double> nominal;

        shared_ptr<string> quantity;
        shared_ptr<string> unit;
        shared_ptr<string> displayUnit;

        shared_ptr<unsigned int> derivative;

        bool reinit;
        bool unbounded;
        bool relativeQuantity;

        void load(const ptree &node);
    };

    struct StringAttribute {
        shared_ptr<string> start;

        void load(const ptree &node);
    };

    struct BooleanAttribute {
        shared_ptr<bool> start;

        void load(const ptree &node);
    };

    struct EnumerationAttribute {
        shared_ptr<int> min;
        shared_ptr<int> max;
        shared_ptr<int> start;

        shared_ptr<string> quantity;

        void load(const ptree &node);
    };

}


#endif //FMICPP_SCALARVARIBALEATTRIBUTES_HPP

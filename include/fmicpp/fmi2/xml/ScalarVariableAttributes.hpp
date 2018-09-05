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

#include <string>
#include <boost/property_tree/ptree.hpp>

using std::string;
using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {

    struct IntegerAttribute {
        int min;
        int max;
        int start;

        string quantity;

        void load(const ptree &node);
    };

    struct RealAttribute {
        double min;
        double max;
        double start;
        double nominal;

        bool reinit;
        bool unbounded;
        bool relativeQuantity;

        string quantity;
        string unit;
        string displayUnit;

        unsigned int derivative;

        void load(const ptree &node);
    };

    struct StringAttribute {
        string start;

        void load(const ptree &node);
    };

    struct BooleanAttribute {
        bool start;

        void load(const ptree &node);
    };

    struct EnumerationAttribute {
        int min;
        int max;
        int start;

        string quantity;

        void load(const ptree &node);
    };

}


#endif //FMICPP_SCALARVARIBALEATTRIBUTES_HPP

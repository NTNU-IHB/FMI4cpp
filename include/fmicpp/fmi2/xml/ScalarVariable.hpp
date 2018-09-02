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

#ifndef FMICPP_SCALARVARIABLE_HPP
#define FMICPP_SCALARVARIABLE_HPP

#include <memory>
#include <boost/property_tree/ptree.hpp>

#include "../fmi2Functions.h"

using std::string;
using std::unique_ptr;
using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {
    
    struct IntegerAttribute {
        int min;
        int max;
        int start;
        void load(const ptree &node);
    };

    struct RealAttribute {
        double min;
        double max;
        double start;
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
        void load(const ptree &node);
    };

    //forward declarations
    struct IntegerVariable;
    struct RealVariable;
    struct StringVariable;
    struct BooleanVariable;
    struct EnumerationVariable;
    
    struct ScalarVariable {
        
        fmi2ValueReference valueReference;
        string name;
        string description;

        IntegerVariable asIntegerVariable();
        RealVariable asRealVariable();
        StringVariable asStringVariable();
        BooleanVariable asBooleanVariable();
        EnumerationVariable asEnumerationVariable();

        void load(const ptree &node);

    private:
        unique_ptr<IntegerAttribute> integerAttribute = nullptr;
        unique_ptr<RealAttribute> realAttribute = nullptr;
        unique_ptr<StringAttribute> stringAttribute = nullptr;
        unique_ptr<BooleanAttribute> booleanAttribute = nullptr;
        unique_ptr<EnumerationAttribute> enumerationAttribute = nullptr;

    };

    struct IntegerVariable : ScalarVariable {

        const int min;
        const int max;
        int start;

        IntegerVariable(const IntegerAttribute &attribute);


    };

    struct RealVariable : ScalarVariable {

        const double min;
        const double max;
        double start;

        RealVariable(const RealAttribute &attribute);

    };

    struct StringVariable : ScalarVariable {

        string start;

        StringVariable(const StringAttribute &attribute);

    };

    struct BooleanVariable : ScalarVariable {

        bool start;

        BooleanVariable(const BooleanAttribute &attribute);

    };

    struct EnumerationVariable : ScalarVariable {

        const int min;
        const int max;
        int start;

        EnumerationVariable(const EnumerationAttribute &attribute);

    };
    
}

#endif //FMICPP_SCALARVARIABLE_HPP

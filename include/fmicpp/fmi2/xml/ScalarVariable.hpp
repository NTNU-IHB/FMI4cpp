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

using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {
    
    typedef unsigned int ValueReference;

    struct IntegerAttribute {

        int min;
        int max;
        int start;

        void load(ptree &node);
        
    };

    struct RealAttribute {
        
        double min;
        double max;
        double start;
        
        void load(ptree &node);
        
    };

    struct StringAttribute {
        
        std::string start;

        void load(ptree &node);
        
    };

    struct BooleanAttribute {
        
        bool start;

        void load(ptree &node);

    };

    struct IntegerVariable;
    struct RealVariable;


    struct ScalarVariable {
        
        ValueReference valueReference;
        std::string name;
        std::string description;

        std::unique_ptr<IntegerAttribute> integerAttribute = nullptr;
        std::unique_ptr<RealAttribute> realAttribute = nullptr;
        std::unique_ptr<StringAttribute> stringAttribute = nullptr;
        std::unique_ptr<BooleanAttribute> booleanAttribute = nullptr;
        
        void load(ptree &node);

        IntegerVariable asIntegerVariable();
        RealVariable asRealVariable();

    };

    struct IntegerVariable : ScalarVariable {

        IntegerVariable(IntegerAttribute &attribute);

        int min;
        int max;
        int start;

    };

    struct RealVariable : ScalarVariable {

        RealVariable(RealAttribute &attribute);

        double min;
        double max;
        double start;

    };
    
}

#endif //FMICPP_SCALARVARIABLE_HPP

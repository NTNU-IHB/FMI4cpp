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

#include "enums.hpp"
#include "../fmi2Functions.h"
#include "ScalarVariableAttributes.hpp"

using std::string;
using std::shared_ptr;
using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {

    //forward declarations
    class IntegerVariable;
    class RealVariable;
    class StringVariable;
    class BooleanVariable;
    class EnumerationVariable;

    class ScalarVariable {

    private:
        string name_;
        string description_;
        fmi2ValueReference valueReference_;
        bool canHandleMultipleSetPerTimelnstant_;

        fmi2Causality causality_;
        fmi2Variability variability_;
        fmi2Initial initial_;

        shared_ptr<IntegerAttribute> integerAttribute_;
        shared_ptr<RealAttribute> realAttribute_;
        shared_ptr<StringAttribute> stringAttribute_;
        shared_ptr<BooleanAttribute> booleanAttribute_;
        shared_ptr<EnumerationAttribute> enumerationAttribute_;

    public:

        string getName() const;
        string getDescription() const;
        fmi2ValueReference getValueReference() const;
        bool canHandleMultipleSetPerTimelnstant() const;

        fmi2Causality getCausality() const;
        fmi2Variability getVariability() const;
        fmi2Initial getInitial() const;

        IntegerVariable asIntegerVariable();
        RealVariable asRealVariable();
        StringVariable asStringVariable();
        BooleanVariable asBooleanVariable();
        EnumerationVariable asEnumerationVariable();

        void load(const ptree &node);

    };

    class IntegerVariable : public ScalarVariable {

    private:
        IntegerAttribute attribute_;

    public:
        IntegerVariable(const IntegerAttribute attribute);

        int getMin() const;
        int getMax() const;
        int getStart() const;
        void setStart(const int start);

        string getQuantity() const;

    };

    class RealVariable : public ScalarVariable {

    private:
        RealAttribute attribute_;

    public:
        RealVariable(const RealAttribute attribute);

        double getMin() const;
        double getMax() const;

        double getStart() const;
        void setStart(const double start);

        double getNominal() const;

        bool getReinit() const;
        bool getUnbounded() const;
        bool getRelativeQuantity() const;

        string getQuantity() const;
        string getUnit() const;
        string getDisplayUnit() const;

        unsigned int getDerivative() const;

    };

    class StringVariable : public ScalarVariable {

    private:
        StringAttribute attribute_;

    public:
        StringVariable(const StringAttribute attribute);

        string getStart() const;

        void setStart(const string start);

    };

    class BooleanVariable : public ScalarVariable {

    private:
         BooleanAttribute attribute_;

    public:
        BooleanVariable(const BooleanAttribute attribute);

        bool getStart() const;

        void setStart(const bool start);

    };

    class EnumerationVariable : public ScalarVariable {

    private:
        EnumerationAttribute attribute_;

    public:
        EnumerationVariable(const EnumerationAttribute attribute);

        int getMin() const;
        int getMax() const;
        int getStart() const;
        void setStart(const int start);

        string getQuantity() const;

    };
    
}

#endif //FMICPP_SCALARVARIABLE_HPP

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
        IntegerAttribute &attribute_;

    public:
        IntegerVariable(ScalarVariable &var, IntegerAttribute &attribute);

        shared_ptr<int> getMin() const;
        shared_ptr<int> getMax() const;

        shared_ptr<int> getStart() const;
        void setStart(const int start);

        shared_ptr<string> getQuantity() const;

    };

    class RealVariable : public ScalarVariable {

    private:
        RealAttribute &attribute_;

    public:
        RealVariable(ScalarVariable &var, RealAttribute &attribute);

        shared_ptr<double> getMin() const;
        shared_ptr<double> getMax() const;

        shared_ptr<double> getStart() const;
        void setStart(const double start);

        shared_ptr<double> getNominal() const;

        bool getReinit() const;
        bool getUnbounded() const;
        bool getRelativeQuantity() const;

        shared_ptr<string> getQuantity() const;
        shared_ptr<string> getUnit() const;
        shared_ptr<string> getDisplayUnit() const;

        shared_ptr<unsigned int> getDerivative() const;

    };

    class StringVariable : public ScalarVariable {

    private:
        StringAttribute attribute_;

    public:
        StringVariable(ScalarVariable &var, StringAttribute &attribute);

        shared_ptr<string> getStart() const;

        void setStart(shared_ptr<string> start);

    };

    class BooleanVariable : public ScalarVariable {

    private:
         BooleanAttribute attribute_;

    public:
        BooleanVariable(ScalarVariable &var, BooleanAttribute &attribute);

        shared_ptr<bool> getStart() const;

        void setStart(const bool start);

    };

    class EnumerationVariable : public ScalarVariable {

    private:
        EnumerationAttribute attribute_;

    public:
        EnumerationVariable(ScalarVariable &var, EnumerationAttribute &attribute);

        shared_ptr<int> getMin() const;
        shared_ptr<int> getMax() const;
        shared_ptr<int> getStart() const;
        void setStart(const int start);

        shared_ptr<string> getQuantity() const;

    };
    
}

#endif //FMICPP_SCALARVARIABLE_HPP

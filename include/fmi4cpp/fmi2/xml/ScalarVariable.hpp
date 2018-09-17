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

#ifndef FMI4CPP_SCALARVARIABLE_HPP
#define FMI4CPP_SCALARVARIABLE_HPP

#include <memory>
#include <optional>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

#include "enums.hpp"
#include "../fmi2Functions.h"
#include "ScalarVariableAttributes.hpp"

using std::string;
using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

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

        std::shared_ptr<IntegerAttribute> integerAttribute_;
        std::shared_ptr<RealAttribute> realAttribute_;
        std::shared_ptr<StringAttribute> stringAttribute_;
        std::shared_ptr<BooleanAttribute> booleanAttribute_;
        std::shared_ptr<EnumerationAttribute> enumerationAttribute_;

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
        IntegerVariable(const ScalarVariable &var, IntegerAttribute &attribute);

        std::optional<int> getMin() const;

        std::optional<int> getMax() const;

        std::optional<int> getStart() const;

        void setStart(const int start);

        std::optional<string> getQuantity() const;

    };

    class RealVariable : public ScalarVariable {

    private:
        RealAttribute &attribute_;

    public:
        RealVariable(const ScalarVariable &var, RealAttribute &attribute);

        std::optional<double> getMin() const;

        std::optional<double> getMax() const;

        std::optional<double> getStart() const;

        void setStart(const double start);

        std::optional<double> getNominal() const;

        bool getReinit() const;

        bool getUnbounded() const;

        bool getRelativeQuantity() const;

        std::optional<string> getQuantity() const;

        std::optional<string> getUnit() const;

        std::optional<string> getDisplayUnit() const;

        std::optional<unsigned int> getDerivative() const;

    };

    class StringVariable : public ScalarVariable {

    private:
        StringAttribute attribute_;

    public:
        StringVariable(const ScalarVariable &var, StringAttribute &attribute);

        std::optional<string> getStart() const;

        void setStart(const string &start);

    };

    class BooleanVariable : public ScalarVariable {

    private:
        BooleanAttribute attribute_;

    public:
        BooleanVariable(const ScalarVariable &var, BooleanAttribute &attribute);

        std::optional<bool> getStart() const;

        void setStart(const bool start);

    };

    class EnumerationVariable : public ScalarVariable {

    private:
        EnumerationAttribute attribute_;

    public:
        EnumerationVariable(const ScalarVariable &var, EnumerationAttribute &attribute);

        std::optional<int> getMin() const;

        std::optional<int> getMax() const;

        std::optional<int> getStart() const;

        void setStart(const int start);

        std::optional<string> getQuantity() const;

    };

}

#endif //FMI4CPP_SCALARVARIABLE_HPP

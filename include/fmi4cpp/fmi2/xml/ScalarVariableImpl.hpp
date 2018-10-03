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

#ifndef FMI4CPP_SCALARVARIABLEIMPL_HPP
#define FMI4CPP_SCALARVARIABLEIMPL_HPP

#include "ScalarVariable.hpp"

namespace fmi4cpp::fmi2::xml {

    class ScalarVariableImpl: public virtual ScalarVariable {

    private:
        std::string name_;
        std::string description_;
        fmi2ValueReference valueReference_;
        bool canHandleMultipleSetPerTimelnstant_;

        fmi2Initial initial_;
        fmi2Causality causality_;
        fmi2Variability variability_;

        std::shared_ptr<IntegerAttribute> integerAttribute_;
        std::shared_ptr<RealAttribute> realAttribute_;
        std::shared_ptr<StringAttribute> stringAttribute_;
        std::shared_ptr<BooleanAttribute> booleanAttribute_;
        std::shared_ptr<EnumerationAttribute> enumerationAttribute_;

    public:

        ScalarVariableImpl(const ptree &node);

        std::string getName() const override;

        std::string getDescription() const override;

        fmi2ValueReference getValueReference() const override;

        bool canHandleMultipleSetPerTimelnstant() const override;

        fmi2Causality getCausality() const override;

        fmi2Variability getVariability() const override;

        fmi2Initial getInitial() const override;

        IntegerVariable asIntegerVariable() const override;

        RealVariable asRealVariable() const override;

        StringVariable asStringVariable() const override;

        BooleanVariable asBooleanVariable() const override;

        EnumerationVariable asEnumerationVariable() const override;

        bool isIntegerVariable() const override;

        bool isRealVariable() const override;

        bool isStringVariable() const override;

        bool isBooleanVariable() const override;

        bool isEnumerationVariable() const override;

        std::string toString() const override;

    };


    template <class T, class U>
    class TypedVariable {

    protected:
        const U &attribute_;

    public:

        explicit TypedVariable(const U &attribute_) : attribute_(attribute_) {}

        std::optional<T> getStart() const {
            return attribute_.start;
        }

    };

    template <class T, class U>
    class BoundedVariable: public TypedVariable<T, U> {

    public:

        explicit BoundedVariable(const U &attribute_) : TypedVariable(attribute_) {}

        std::optional<T> getMin() const {
            return attribute_.min;
        }

        std::optional<T> getMax() const {
            return attribute_.max;
        }

        std::optional<string> getQuantity() const {
            return attribute_.quantity;
        }

    };


    class IntegerVariable : public ScalarVariableImpl, public BoundedVariable<int, IntegerAttribute> {

    public:
        IntegerVariable(const ScalarVariableImpl &var, const IntegerAttribute &attribute);
    };

    class RealVariable : public ScalarVariableImpl, public BoundedVariable<double, RealAttribute> {

    public:
        RealVariable(const ScalarVariableImpl &var, const RealAttribute &attribute);

        std::optional<double> getNominal() const;

        bool getReinit() const;

        bool getUnbounded() const;

        bool getRelativeQuantity() const;

        std::optional<string> getUnit() const;

        std::optional<string> getDisplayUnit() const;

        std::optional<unsigned int> getDerivative() const;

    };

    class StringVariable : public ScalarVariableImpl, public TypedVariable<string, StringAttribute> {

    public:
        StringVariable(const ScalarVariableImpl &var, const StringAttribute &attribute);
    };

    class BooleanVariable :  public ScalarVariableImpl, public TypedVariable<bool, BooleanAttribute> {

    public:
        BooleanVariable(const ScalarVariableImpl &var, const BooleanAttribute &attribute);
    };

    class EnumerationVariable : public ScalarVariableImpl, public BoundedVariable<int, EnumerationAttribute> {

    public:
        EnumerationVariable(const ScalarVariableImpl &var, const EnumerationAttribute &attribute);

    };


}


#endif //FMI4CPP_SCALARVARIABLEIMPL_HPP

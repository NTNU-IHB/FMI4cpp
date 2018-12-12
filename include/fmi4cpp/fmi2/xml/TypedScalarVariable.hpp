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

#ifndef FMI4CPP_TYPEDSCALARVARIABLE_HPP
#define FMI4CPP_TYPEDSCALARVARIABLE_HPP

#include "ScalarVariable.hpp"
#include "fmi4cpp/FmuVariableAccessor.hpp"

namespace fmi4cpp::fmi2 {

    template<typename T, typename U>
    class TypedScalarVariable {

    private:
        const ScalarVariable variable_;

    protected:
        const U attribute_;

    public:

        TypedScalarVariable(const ScalarVariable &variable, const U &attribute)
                : variable_(variable), attribute_(attribute) {}

        std::string name() const {
            return variable_.name();
        }

        std::string description() const {
            return variable_.description();
        }

        fmi2ValueReference valueReference() const {
            return variable_.valueReference();
        }

        Causality causality() const {
            return variable_.causality();
        }

        Variability variability() const {
            return variable_.variability();
        }

        Initial initial() const {
            return variable_.initial();
        }

        bool canHandleMultipleSetPerTimelnstant() const {
            return variable_.canHandleMultipleSetPerTimelnstant();
        }

        std::optional<T> start() const {
            return attribute_.start;
        }

        std::optional<std::string> declaredType() const {
            return attribute_.declaredType;
        }

        const U &attribute() const {
            return attribute_;
        }

        virtual bool read(FmuReader &reader, T &ref) = 0;

        virtual bool write(FmuWriter &writer, T value) = 0;

    };

    template<typename T, typename U>
    class BoundedScalarVariable : public TypedScalarVariable<T, U> {

    public:
        BoundedScalarVariable(const ScalarVariable &variable, const U &attribute)
                : TypedScalarVariable<T, U>(variable, attribute) {}

        std::optional<T> min() const {
            return this->attribute_.min;
        }

        std::optional<T> max() const {
            return this->attribute_.max;
        }

        std::optional<std::string> quantity() const {
            return this->attribute_.quantity;
        }

    };

    class IntegerVariable : public BoundedScalarVariable<int, IntegerAttribute> {

    public:
        IntegerVariable(const ScalarVariable &variable, const IntegerAttribute &attribute);

        bool read(FmuReader &reader, int &ref) override;

        bool write(FmuWriter &writer, int value) override;

    };

    class RealVariable : public BoundedScalarVariable<double, RealAttribute> {

    public:

        RealVariable(const ScalarVariable &variable, const RealAttribute &attribute);

        bool reinit() const;

        bool unbounded() const;

        bool relativeQuantity() const;

        std::optional<double> nominal() const;

        std::optional<size_t> derivative() const;

        std::optional<std::string> unit() const;

        std::optional<std::string> displayUnit() const;

        bool read(FmuReader &reader, double &ref) override;

        bool write(FmuWriter &writer, double value) override;

    };


    class StringVariable : public TypedScalarVariable<std::string, StringAttribute> {

    public:
        StringVariable(const ScalarVariable &variable, const StringAttribute &attribute);

        bool read(FmuReader &reader, std::string &ref) override;

        bool write(FmuWriter &writer, std::string value) override;

    };

    class BooleanVariable : public TypedScalarVariable<bool, BooleanAttribute> {

    public:
        BooleanVariable(const ScalarVariable &variable, const BooleanAttribute &attribute);

        bool read(FmuReader &reader, bool &ref) override;

        bool write(FmuWriter &writer, bool value) override;

    };

    class EnumerationVariable : public TypedScalarVariable<int, EnumerationAttribute> {

    public:
        EnumerationVariable(const ScalarVariable &variable, const EnumerationAttribute &attribute);

        bool read(FmuReader &reader, int &ref) override;

        bool write(FmuWriter &writer, int value) override;

    };


}


#endif //FMI4CPP_TYPEDSCALARVARIABLE_HPP

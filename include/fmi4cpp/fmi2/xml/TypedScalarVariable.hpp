//
// Created by LarsIvar on 12.10.2018.
//

#ifndef FMI4CPP_TYPEDSCALARVARIABLE_HPP
#define FMI4CPP_TYPEDSCALARVARIABLE_HPP

#include "ScalarVariable.hpp"
#include "../import/FmuInstance.hpp"

namespace fmi4cpp::fmi2::xml {

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

        fmi2Causality causality() const {
            return variable_.causality();
        }

        fmi2Variability variability() const {
            return variable_.variability();
        }

        fmi2Initial initial() const {
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

        virtual fmi2Status read(const import::FmuReader &reader, T &ref) = 0;

        virtual fmi2Status write(import::FmuWriter &writer, T value) = 0;

    };

    template<typename T, typename U>
    class BoundedScalarVariable : public TypedScalarVariable<T, U> {

    public:
        BoundedScalarVariable(const ScalarVariable &variable, const U &attribute)
                : TypedScalarVariable(variable, attribute) {}

        std::optional<T> min() const {
            return attribute_.min;
        }

        std::optional<T> max() const {
            return attribute_.max;
        }

        std::optional<std::string> quantity() const {
            return attribute_.quantity;
        }

    };

    class IntegerVariable : public BoundedScalarVariable<int, IntegerAttribute> {

    public:
        IntegerVariable(const ScalarVariable &variable, const IntegerAttribute &attribute);

        fmi2Status read(const import::FmuReader &reader, int &ref) override;

        fmi2Status write(import::FmuWriter &writer, int value) override;

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

        fmi2Status read(const import::FmuReader &reader, double &ref) override;

        fmi2Status write(import::FmuWriter &writer, double value) override;

    };


    class StringVariable : public TypedScalarVariable<std::string, StringAttribute> {

    public:
        StringVariable(const ScalarVariable &variable, const StringAttribute &attribute);

        fmi2Status read(const import::FmuReader &reader, std::string &ref) override;

        fmi2Status write(import::FmuWriter &writer, std::string value) override;

    };

    class BooleanVariable : public TypedScalarVariable<bool, BooleanAttribute> {

    public:
        BooleanVariable(const ScalarVariable &variable, const BooleanAttribute &attribute);

        fmi2Status read(const import::FmuReader &reader, bool &ref) override;

        fmi2Status write(import::FmuWriter &writer, bool value) override;

    };

    class EnumerationVariable : public TypedScalarVariable<int, EnumerationAttribute> {

    public:
        EnumerationVariable(const ScalarVariable &variable, const EnumerationAttribute &attribute);

        fmi2Status read(const import::FmuReader &reader, int &ref) override;

        fmi2Status write(import::FmuWriter &writer, int value) override;

    };


}


#endif //FMI4CPP_TYPEDSCALARVARIABLE_HPP

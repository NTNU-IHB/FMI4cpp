
#ifndef FMI4CPP_TYPEDSCALARVARIABLE_HPP
#define FMI4CPP_TYPEDSCALARVARIABLE_HPP

#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>
#include <fmi4cpp/fmu_variable_accessor.hpp>

#include <utility>


namespace fmi4cpp::fmi2
{

template<typename T, typename U>
class typed_scalar_variable
{

protected:
    const U attribute_;

private:
    const scalar_variable variable_;


public:
    typed_scalar_variable(
        scalar_variable variable,
        const U& attribute)
        : variable_(std::move(variable))
        , attribute_(attribute)
    { }

    [[nodiscard]] std::string name() const
    {
        return variable_.name;
    }

    [[nodiscard]] std::string description() const
    {
        return variable_.description;
    }

    [[nodiscard]] fmi2ValueReference valueReference() const
    {
        return variable_.value_reference;
    }

    [[nodiscard]] fmi2::causality causality() const
    {
        return variable_.causality;
    }

    [[nodiscard]] fmi2::variability variability() const
    {
        return variable_.variability;
    }

    [[nodiscard]] fmi2::initial initial() const
    {
        return variable_.initial;
    }

    [[nodiscard]] bool canHandleMultipleSetPerTimelnstant() const
    {
        return variable_.can_handle_multiple_set_per_time_instant;
    }

    [[nodiscard]] std::optional<T> start() const
    {
        return attribute_.start;
    }

    [[nodiscard]] std::optional<std::string> declaredType() const
    {
        return attribute_.declaredType;
    }

    [[nodiscard]] const U& attribute() const
    {
        return attribute_;
    }
};

template<typename T, typename U>
class bounded_scalar_variable : public typed_scalar_variable<T, U>
{

public:
    bounded_scalar_variable(const scalar_variable& variable, const U& attribute)
        : typed_scalar_variable<T, U>(variable, attribute)
    { }

#if !defined(_WIN32) || (defined(_WIN32) && defined(NOMINMAX))
    [[deprecated("Use get_min() instead")]] [[nodiscard]] std::optional<T> min() const
    {
        return this->attribute_.min;
    }

    [[deprecated("Use get_max() instead")]] [[nodiscard]] std::optional<T> max() const
    {
        return this->attribute_.max;
    }
#endif

    [[nodiscard]] std::optional<T> get_min() const
    {
        return this->attribute_.min;
    }

    [[nodiscard]] std::optional<T> get_max() const
    {
        return this->attribute_.max;
    }

    [[nodiscard]] std::optional<std::string> quantity() const
    {
        return this->attribute_.quantity;
    }
};

class integer_variable : public bounded_scalar_variable<int, integer_attribute>
{

public:
    integer_variable(
        const scalar_variable& variable,
        const integer_attribute& attribute);
};

class real_variable : public bounded_scalar_variable<double, real_attribute>
{

public:
    real_variable(
        const scalar_variable& variable,
        const real_attribute& attribute);

    [[nodiscard]] bool reinit() const;
    [[nodiscard]] bool unbounded() const;
    [[nodiscard]] bool relativeQuantity() const;

    [[nodiscard]] std::optional<double> nominal() const;
    [[nodiscard]] std::optional<unsigned int> derivative() const;
    [[nodiscard]] std::optional<std::string> unit() const;
    [[nodiscard]] std::optional<std::string> displayUnit() const;
};


class string_variable : public typed_scalar_variable<std::string, string_attribute>
{

public:
    string_variable(
        const scalar_variable& variable,
        const string_attribute& attribute);
};

class boolean_variable : public typed_scalar_variable<bool, boolean_attribute>
{

public:
    boolean_variable(
        const scalar_variable& variable,
        const boolean_attribute& attribute);
};

class enumeration_variable : public typed_scalar_variable<int, enumeration_attribute>
{

public:
    enumeration_variable(
        const scalar_variable& variable,
        const enumeration_attribute& attribute);
};


} // namespace fmi4cpp::fmi2


#endif // FMI4CPP_TYPEDSCALARVARIABLE_HPP

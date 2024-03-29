
#ifndef FMI4CPP_SCALARVARIABLE_HPP
#define FMI4CPP_SCALARVARIABLE_HPP

#include <fmi4cpp/fmi2/fmi2Functions.h>
#include <fmi4cpp/fmi2/xml/enums.hpp>

#include <optional>

namespace fmi4cpp::fmi2
{

class integer_variable;
class real_variable;
class string_variable;
class boolean_variable;
class enumeration_variable;

const std::string INTEGER_TYPE = "Integer";
const std::string REAL_TYPE = "Real";
const std::string STRING_TYPE = "String";
const std::string BOOLEAN_TYPE = "Boolean";
const std::string ENUMERATION_TYPE = "Enumeration";
const std::string UNKNOWN_TYPE = "Unknown";

struct scalar_variable_base
{
    std::string name;
    std::string description;

    fmi2::initial initial;
    fmi2::causality causality;
    fmi2::variability variability;

    fmi2ValueReference value_reference;
    bool can_handle_multiple_set_per_time_instant;
};


template<typename T>
struct scalar_variable_attribute
{
    std::optional<T> start;
    std::optional<std::string> declared_type;
};


template<typename T>
struct bounded_scalar_variable_attribute : scalar_variable_attribute<T>
{
    std::optional<T> min;
    std::optional<T> max;
    std::optional<std::string> quantity;

    explicit bounded_scalar_variable_attribute(const scalar_variable_attribute<T>& attributes)
        : scalar_variable_attribute<T>(attributes)
    {}
};

struct integer_attribute : bounded_scalar_variable_attribute<int>
{
    explicit integer_attribute(const bounded_scalar_variable_attribute<int>& attributes);
};

struct real_attribute : bounded_scalar_variable_attribute<double>
{
    bool reinit = false;
    bool unbounded = false;
    bool relative_quantity = false;

    std::optional<double> nominal;
    std::optional<unsigned int> derivative;

    std::optional<std::string> unit;
    std::optional<std::string> display_unit;

    explicit real_attribute(const bounded_scalar_variable_attribute<double>& attributes);
};

struct string_attribute : scalar_variable_attribute<std::string>
{
    explicit string_attribute(const scalar_variable_attribute<std::string>& attributes);
};

struct boolean_attribute : scalar_variable_attribute<bool>
{
    explicit boolean_attribute(const scalar_variable_attribute<bool>& attributes);
};

struct enumeration_attribute : bounded_scalar_variable_attribute<int>
{
    explicit enumeration_attribute(const bounded_scalar_variable_attribute<int>& attributes);
};


class scalar_variable : public scalar_variable_base
{

private:
    std::optional<integer_attribute> integer_;
    std::optional<real_attribute> real_;
    std::optional<string_attribute> string_;
    std::optional<boolean_attribute> boolean_;
    std::optional<enumeration_attribute> enumeration_;

public:
    scalar_variable(const scalar_variable_base& base,
        const integer_attribute& integer);

    scalar_variable(const scalar_variable_base& base,
        const real_attribute& real);

    scalar_variable(const scalar_variable_base& base,
        const string_attribute& string);

    scalar_variable(const scalar_variable_base& base,
        const boolean_attribute& enumeration);

    scalar_variable(const scalar_variable_base& base,
        const enumeration_attribute& enumeration);


    [[nodiscard]] bool is_integer() const;
    [[nodiscard]] bool is_real() const;
    [[nodiscard]] bool is_string() const;
    [[nodiscard]] bool is_boolean() const;
    [[nodiscard]] bool is_enumeration() const;

    [[nodiscard]] std::string type_name() const;

    [[nodiscard]] integer_variable as_integer() const;
    [[nodiscard]] real_variable as_real() const;
    [[nodiscard]] string_variable as_string() const;
    [[nodiscard]] boolean_variable as_boolean() const;
    [[nodiscard]] enumeration_variable as_enumeration() const;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_SCALARVARIABLE_HPP

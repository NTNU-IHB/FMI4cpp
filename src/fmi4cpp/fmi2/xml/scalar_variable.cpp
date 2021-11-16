
#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>
#include <fmi4cpp/fmi2/xml/typed_scalar_variable.hpp>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

scalar_variable::scalar_variable(
    const scalar_variable_base& base,
    const integer_attribute& integer)
    : scalar_variable_base(base)
    , integer_(integer)
{}

scalar_variable::scalar_variable(
    const scalar_variable_base& base,
    const real_attribute& real)
    : scalar_variable_base(base)
    , real_(real)
{}

scalar_variable::scalar_variable(
    const scalar_variable_base& base,
    const string_attribute& string)
    : scalar_variable_base(base)
    , string_(string)
{}

scalar_variable::scalar_variable(
    const scalar_variable_base& base,
    const boolean_attribute& boolean)
    : scalar_variable_base(base)
    , boolean_(boolean)
{}

scalar_variable::scalar_variable(
    const scalar_variable_base& base,
    const enumeration_attribute& enumeration)
    : scalar_variable_base(base)
    , enumeration_(enumeration)
{}


bool scalar_variable::is_integer() const
{
    return integer_.has_value();
}

bool scalar_variable::is_real() const
{
    return real_.has_value();
}

bool scalar_variable::is_string() const
{
    return string_.has_value();
}

bool scalar_variable::is_boolean() const
{
    return boolean_.has_value();
}

bool scalar_variable::is_enumeration() const
{
    return enumeration_.has_value();
}

integer_variable scalar_variable::as_integer() const
{
    return integer_variable(*this, *integer_);
}

real_variable scalar_variable::as_real() const
{
    return real_variable(*this, *real_);
}

string_variable scalar_variable::as_string() const
{
    return string_variable(*this, *string_);
}

boolean_variable scalar_variable::as_boolean() const
{
    return boolean_variable(*this, *boolean_);
}

enumeration_variable scalar_variable::as_enumeration() const
{
    return enumeration_variable(*this, *enumeration_);
}

std::string scalar_variable::type_name() const
{
    if (integer_) {
        return INTEGER_TYPE;
    } else if (real_) {
        return REAL_TYPE;
    } else if (string_) {
        return STRING_TYPE;
    } else if (boolean_) {
        return BOOLEAN_TYPE;
    } else if (enumeration_) {
        return ENUMERATION_TYPE;
    } else {
        return UNKNOWN_TYPE;
    }
}


integer_variable::integer_variable(
    const scalar_variable& variable,
    const integer_attribute& attribute)
    : bounded_scalar_variable(variable, attribute)
{}


real_variable::real_variable(
    const scalar_variable& variable,
    const real_attribute& attribute)
    : bounded_scalar_variable(variable, attribute)
{}

std::optional<std::string> real_variable::displayUnit() const
{
    return attribute_.display_unit;
}

std::optional<std::string> real_variable::unit() const
{
    return attribute_.unit;
}

std::optional<unsigned int> real_variable::derivative() const
{
    return attribute_.derivative;
}

std::optional<double> real_variable::nominal() const
{
    return attribute_.nominal;
}

bool real_variable::relativeQuantity() const
{
    return attribute_.relative_quantity;
}

bool real_variable::unbounded() const
{
    return attribute_.unbounded;
}

bool real_variable::reinit() const
{
    return attribute_.reinit;
}


string_variable::string_variable(
    const scalar_variable& variable,
    const string_attribute& attribute)
    : typed_scalar_variable(variable, attribute)
{}


boolean_variable::boolean_variable(
    const scalar_variable& variable,
    const boolean_attribute& attribute)
    : typed_scalar_variable(variable, attribute)
{}


enumeration_variable::enumeration_variable(
    const scalar_variable& variable,
    const enumeration_attribute& attribute)
    : typed_scalar_variable(variable, attribute)
{}


integer_attribute::integer_attribute(const bounded_scalar_variable_attribute<int>& attributes)
    : bounded_scalar_variable_attribute<int>(attributes)
{}


real_attribute::real_attribute(const bounded_scalar_variable_attribute<double>& attributes)
    : bounded_scalar_variable_attribute<double>(attributes)
{}


string_attribute::string_attribute(const scalar_variable_attribute<std::string>& attributes)
    : scalar_variable_attribute<std::string>(attributes)
{}


boolean_attribute::boolean_attribute(const scalar_variable_attribute<bool>& attributes)
    : scalar_variable_attribute<bool>(attributes)
{}


enumeration_attribute::enumeration_attribute(const bounded_scalar_variable_attribute<int>& attributes)
    : bounded_scalar_variable_attribute<int>(attributes)
{}

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

#include <fmi4cpp/fmi2/fmi2Functions.h>
#include <fmi4cpp/fmi2/xml/enums.hpp>

#include <boost/optional.hpp>


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

    boost::optional<T> start;
    boost::optional<std::string> declared_type;
};


template<typename T>
struct bounded_scalar_variable_attribute : scalar_variable_attribute<T>
{

    boost::optional<T> min;
    boost::optional<T> max;
    boost::optional<std::string> quantity;

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

    boost::optional<double> nominal;
    boost::optional<unsigned int> derivative;

    boost::optional<std::string> unit;
    boost::optional<std::string> display_unit;

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
    boost::optional<integer_attribute> integer_;
    boost::optional<real_attribute> real_;
    boost::optional<string_attribute> string_;
    boost::optional<boolean_attribute> boolean_;
    boost::optional<enumeration_attribute> enumeration_;

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


    bool is_integer() const;

    bool is_real() const;

    bool is_string() const;

    bool is_boolean() const;

    bool is_enumeration() const;

    std::string type_name() const;

    const integer_variable as_integer() const;

    const real_variable as_real() const;

    const string_variable as_string() const;

    const boolean_variable as_boolean() const;

    const enumeration_variable as_enumeration() const;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_SCALARVARIABLE_HPP

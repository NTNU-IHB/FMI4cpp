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

#include <fmi4cpp/fmu_variable_accessor.hpp>
#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>


namespace fmi4cpp::fmi2 {

    template<typename T, typename U>
    class typed_scalar_variable {

    protected:

        const U attribute_;

    private:

        const scalar_variable variable_;


    public:

        typed_scalar_variable(const scalar_variable &variable, const U &attribute)
                : variable_(variable), attribute_(attribute) {}

        std::string name() const {
            return variable_.name;
        }

        std::string description() const {
            return variable_.description;
        }

        fmi2ValueReference valueReference() const {
            return variable_.value_reference;
        }

        fmi2::causality causality() const {
            return variable_.causality;
        }

        fmi2::variability variability() const {
            return variable_.variability;
        }

        fmi2::initial initial() const {
            return variable_.initial;
        }

        bool canHandleMultipleSetPerTimelnstant() const {
            return variable_.can_handle_multiple_set_per_time_instant;
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

        virtual bool read(fmu_reader &reader, T &ref) = 0;

        virtual bool write(fmu_writer &writer, T value) = 0;

    };

    template<typename T, typename U>
    class bounded_scalar_variable : public typed_scalar_variable<T, U> {

    public:
        bounded_scalar_variable(const scalar_variable &variable, const U &attribute)
                : typed_scalar_variable<T, U>(variable, attribute) {}

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

    class integer_variable : public bounded_scalar_variable<int, integer_attribute> {

    public:
        integer_variable(const scalar_variable &variable, const integer_attribute &attribute);

        bool read(fmu_reader &reader, int &ref) override;

        bool write(fmu_writer &writer, int value) override;

    };

    class real_variable : public bounded_scalar_variable<double, real_attribute> {

    public:

        real_variable(const scalar_variable &variable, const real_attribute &attribute);

        bool reinit() const;

        bool unbounded() const;

        bool relativeQuantity() const;

        std::optional<double> nominal() const;

        std::optional<size_t> derivative() const;

        std::optional<std::string> unit() const;

        std::optional<std::string> displayUnit() const;

        bool read(fmu_reader &reader, double &ref) override;

        bool write(fmu_writer &writer, double value) override;

    };


    class string_variable : public typed_scalar_variable<std::string, string_attribute> {

    public:
        string_variable(const scalar_variable &variable, const string_attribute &attribute);

        bool read(fmu_reader &reader, std::string &ref) override;

        bool write(fmu_writer &writer, std::string value) override;

    };

    class boolean_variable : public typed_scalar_variable<bool, boolean_attribute> {

    public:
        boolean_variable(const scalar_variable &variable, const boolean_attribute &attribute);

        bool read(fmu_reader &reader, bool &ref) override;

        bool write(fmu_writer &writer, bool value) override;

    };

    class enumeration_variable : public typed_scalar_variable<int, enumeration_attribute> {

    public:
        enumeration_variable(const scalar_variable &variable, const enumeration_attribute &attribute);

        bool read(fmu_reader &reader, int &ref) override;

        bool write(fmu_writer &writer, int value) override;

    };


}


#endif //FMI4CPP_TYPEDSCALARVARIABLE_HPP

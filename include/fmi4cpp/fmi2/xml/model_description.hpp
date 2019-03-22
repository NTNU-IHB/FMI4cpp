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

#ifndef FMI4CPP_MODELDESCRIPTION_HPP
#define FMI4CPP_MODELDESCRIPTION_HPP

#include <memory>
#include <optional>
#include <string>

#include <fmi4cpp/fmi2/xml/model_structure.hpp>
#include <fmi4cpp/fmi2/xml/model_variables.hpp>
#include <fmi4cpp/fmi2/xml/default_experiment.hpp>
#include <fmi4cpp/fmi2/xml/fmu_attributes.hpp>

namespace fmi4cpp::fmi2 {

    struct model_description_base {

        std::string guid;
        std::string model_name;
        std::string fmi_version;

        std::optional<std::string> author;
        std::optional<std::string> version;
        std::optional<std::string> license;
        std::optional<std::string> copyright;
        std::optional<std::string> description;
        std::optional<std::string> generation_tool;
        std::optional<std::string> generation_date_and_time;
        std::optional<std::string> variable_naming_convention;

        std::shared_ptr<const fmi2::model_variables> model_variables;
        std::shared_ptr<const fmi2::model_structure> model_structure;

        std::optional<fmi2::default_experiment> default_experiment;

        size_t number_of_event_indicators;
        size_t number_of_continuous_states() const;

        unsigned int get_value_reference(const std::string &name) const;

        const scalar_variable &get_variable_by_name(const std::string &name) const;

    };

    struct cs_model_description;
    struct me_model_description;

    class model_description : public model_description_base {

    protected:
        std::optional<const cs_attributes> coSimulation_;
        std::optional<const me_attributes> modelExchange_;

    public:

        model_description(const model_description_base &base,
                         const std::optional<const cs_attributes> &coSimulation,
                         const std::optional<const me_attributes> &modelExchange);

        bool supports_cs() const;

        bool supports_me() const;

        std::unique_ptr<const cs_model_description> as_cs_description() const;

        std::unique_ptr<const me_model_description> as_me_description() const;

    };

}

#endif //FMI4CPP_MODELDESCRIPTION_HPP

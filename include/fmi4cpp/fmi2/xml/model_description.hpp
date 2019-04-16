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
#include <string>

#include <boost/optional.hpp>

#include <fmi4cpp/fmi2/xml/model_structure.hpp>
#include <fmi4cpp/fmi2/xml/model_variables.hpp>
#include <fmi4cpp/fmi2/xml/default_experiment.hpp>
#include <fmi4cpp/fmi2/xml/fmu_attributes.hpp>

namespace fmi4cpp::fmi2 {

    struct model_description_base {

        std::string guid;
        std::string model_name;
        std::string fmi_version;

        boost::optional<std::string> author;
        boost::optional<std::string> version;
        boost::optional<std::string> license;
        boost::optional<std::string> copyright;
        boost::optional<std::string> description;
        boost::optional<std::string> generation_tool;
        boost::optional<std::string> generation_date_and_time;
        boost::optional<std::string> variable_naming_convention;

        std::shared_ptr<const fmi2::model_variables> model_variables;
        std::shared_ptr<const fmi2::model_structure> model_structure;

        boost::optional<fmi2::default_experiment> default_experiment;

        size_t number_of_event_indicators;

        size_t number_of_continuous_states() const;

        unsigned int get_value_reference(const std::string &name) const;

        const scalar_variable &get_variable_by_name(const std::string &name) const;

    };

    struct cs_model_description;
    struct me_model_description;

    class model_description : public model_description_base {

    protected:

        boost::optional<cs_attributes> coSimulation_;
        boost::optional<me_attributes> modelExchange_;

    public:

        model_description(const model_description_base &base,
                          boost::optional<cs_attributes> coSimulation,
                          boost::optional<me_attributes> modelExchange);

        bool supports_cs() const;

        bool supports_me() const;

        std::unique_ptr<const cs_model_description> as_cs_description() const;

        std::unique_ptr<const me_model_description> as_me_description() const;

    };

}

#endif //FMI4CPP_MODELDESCRIPTION_HPP

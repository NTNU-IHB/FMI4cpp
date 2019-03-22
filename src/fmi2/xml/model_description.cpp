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

#include <fmi4cpp/fmi2/xml/model_description.hpp>

#include <fmi4cpp/fmi2/xml/cs_model_description.hpp>
#include <fmi4cpp/fmi2/xml/me_model_description.hpp>

using namespace fmi4cpp::fmi2;

size_t model_description_base::number_of_continuous_states() const {
    return model_structure->derivatives.size();
}

const scalar_variable &model_description_base::get_variable_by_name(const std::string &name) const {
    return model_variables->getByName(name);
}

fmi2ValueReference model_description_base::get_value_reference(const std::string &name) const {
    return model_variables->getByName(name).value_reference;
}

model_description::model_description(const model_description_base &base,
                                   const std::optional<const cs_attributes> &coSimulation,
                                   const std::optional<const me_attributes> &modelExchange)
        : model_description_base(base), coSimulation_(coSimulation), modelExchange_(modelExchange) {}

bool model_description::supports_cs() const {
    return coSimulation_.has_value();
}

bool model_description::supports_me() const {
    return modelExchange_.has_value();
}

std::unique_ptr<const cs_model_description> model_description::as_cs_description() const {
    if (!supports_cs()) {
        throw std::runtime_error("CoSimulation not supported!");
    }
    return std::make_unique<cs_model_description>(*this, *coSimulation_);
}

std::unique_ptr<const me_model_description> model_description::as_me_description() const {
    if (!supports_me()) {
        throw std::runtime_error("ModelExchange not supported!");
    }
    return std::make_unique<me_model_description>(*this, *modelExchange_);
}

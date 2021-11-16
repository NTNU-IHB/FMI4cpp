
#include <fmi4cpp/fmi2/xml/cs_model_description.hpp>
#include <fmi4cpp/fmi2/xml/me_model_description.hpp>

#include <utility>
#include <stdexcept>

using namespace fmi4cpp::fmi2;

size_t model_description_base::number_of_continuous_states() const
{
    return model_structure->derivatives.size();
}

const scalar_variable& model_description_base::get_variable_by_name(const std::string& name) const
{
    return model_variables->getByName(name);
}

fmi2ValueReference model_description_base::get_value_reference(const std::string& name) const
{
    return model_variables->getByName(name).value_reference;
}

model_description::model_description(
    const model_description_base& base,
    std::optional<cs_attributes> coSimulation,
    std::optional<me_attributes> modelExchange)
    : model_description_base(base)
    , coSimulation_(std::move(coSimulation))
    , modelExchange_(std::move(modelExchange))
{}

bool model_description::supports_cs() const
{
    return coSimulation_.has_value();
}

bool model_description::supports_me() const
{
    return modelExchange_.has_value();
}

std::unique_ptr<const cs_model_description> model_description::as_cs_description() const
{
    if (!supports_cs()) {
        throw std::runtime_error("CoSimulation not supported!");
    }
    return std::make_unique<cs_model_description>(*this, *coSimulation_);
}

std::unique_ptr<const me_model_description> model_description::as_me_description() const
{
    if (!supports_me()) {
        throw std::runtime_error("ModelExchange not supported!");
    }
    return std::make_unique<me_model_description>(*this, *modelExchange_);
}

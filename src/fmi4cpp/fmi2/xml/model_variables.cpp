
#include <fmi4cpp/fmi2/xml/enums.hpp>
#include <fmi4cpp/fmi2/xml/model_variables.hpp>

#include <stdexcept>
#include <utility>

using namespace fmi4cpp::fmi2;

model_variables::model_variables() = default;

model_variables::model_variables(std::vector<scalar_variable> variables)
    : variables_(std::move(variables))
{}

const scalar_variable& model_variables::getByName(const std::string& name) const
{
    for (auto& v : variables_) {
        if (v.name == name) {
            return v;
        }
    }
    throw std::runtime_error("No such variable with name '" + name + "'!");
}

const scalar_variable& model_variables::getByValueReference(const fmi2ValueReference vr) const
{
    for (const auto& var : variables_) {
        if (var.value_reference == vr) {
            return var;
        }
    }
    throw std::runtime_error("No such variable with valueReference '" + std::to_string(vr) + "'!");
}

void model_variables::getByValueReference(
    const fmi2ValueReference vr,
    std::vector<scalar_variable>& store) const
{
    for (const auto& var : variables_) {
        if (var.value_reference == vr) {
            store.push_back(var);
        }
    }
}

void model_variables::getByCausality(
    const causality causality,
    std::vector<scalar_variable>& store) const
{
    for (const auto& var : variables_) {
        if (var.causality == causality) {
            store.push_back(var);
        }
    }
}

size_t model_variables::size() const
{
    return variables_.size();
}

const scalar_variable& model_variables::operator[](const size_t index) const
{
    return variables_[index];
}

std::vector<scalar_variable>::const_iterator model_variables::begin() const
{
    return variables_.begin();
}

std::vector<scalar_variable>::const_iterator model_variables::end() const
{
    return variables_.end();
}

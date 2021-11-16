
#ifndef FMI4CPP_MODELDESCRIPTION_HPP
#define FMI4CPP_MODELDESCRIPTION_HPP

#include <fmi4cpp/fmi2/xml/default_experiment.hpp>
#include <fmi4cpp/fmi2/xml/fmu_attributes.hpp>
#include <fmi4cpp/fmi2/xml/model_structure.hpp>
#include <fmi4cpp/fmi2/xml/model_variables.hpp>

#include <memory>
#include <optional>
#include <string>

namespace fmi4cpp::fmi2
{

struct model_description_base
{
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
    [[nodiscard]] size_t number_of_continuous_states() const;

    [[nodiscard]] unsigned int get_value_reference(const std::string& name) const;
    [[nodiscard]] const scalar_variable& get_variable_by_name(const std::string& name) const;
};

struct cs_model_description;
struct me_model_description;

class model_description : public model_description_base
{

protected:
    std::optional<cs_attributes> coSimulation_;
    std::optional<me_attributes> modelExchange_;

public:
    model_description(
        const model_description_base& base,
        std::optional<cs_attributes> coSimulation,
        std::optional<me_attributes> modelExchange);

    [[nodiscard]] bool supports_cs() const;
    [[nodiscard]] bool supports_me() const;

    [[nodiscard]] std::unique_ptr<const cs_model_description> as_cs_description() const;
    [[nodiscard]] std::unique_ptr<const me_model_description> as_me_description() const;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_MODELDESCRIPTION_HPP


#ifndef FMI4CPP_FMUTYPESATTRIBUTE_HPP
#define FMI4CPP_FMUTYPESATTRIBUTE_HPP

#include <fmi4cpp/fmi2/xml/source_files.hpp>

#include <string>

namespace fmi4cpp::fmi2
{

struct fmu_attributes
{
    source_files sourceFiles;
    std::string model_identifier;

    bool can_get_and_set_fmu_state;
    bool can_serialize_fmu_state;
    bool needs_execution_tool;
    bool can_not_use_memory_management_functions;
    bool can_be_instantiated_only_once_per_process;
    bool provides_directional_derivative;
};

struct cs_attributes : fmu_attributes
{
    bool can_interpolate_inputs = false;
    bool can_run_asynchronuously = false;
    bool can_handle_variable_communication_step_size = false;

    unsigned int max_output_derivative_order{};

    cs_attributes() = default;

    explicit cs_attributes(const fmu_attributes& attributes)
        : fmu_attributes(attributes)
    {}
};

struct me_attributes : fmu_attributes
{

    bool completed_integrator_step_not_needed = false;

    me_attributes() = default;

    explicit me_attributes(const fmu_attributes& attributes)
        : fmu_attributes(attributes)
    {}
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMUTYPESATTRIBUTE_HPP

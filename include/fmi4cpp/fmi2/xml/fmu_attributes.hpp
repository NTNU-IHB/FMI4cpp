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

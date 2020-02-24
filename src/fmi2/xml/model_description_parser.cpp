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


#include <fmi4cpp/fmi2/xml/model_description_parser.hpp>
#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using boost::property_tree::ptree;
using namespace fmi4cpp::fmi2;

namespace
{

const char* DEFAULT_VARIABLE_NAMING_CONVENTION = "flat";

default_experiment parse_default_experiment(const ptree& node)
{
    default_experiment ex;
    ex.startTime = node.get_optional<double>("<xmlattr>.startTime");
    ex.stopTime = node.get_optional<double>("<xmlattr>.stopTime");
    ex.stepSize = node.get_optional<double>("<xmlattr>.stepSize");
    ex.tolerance = node.get_optional<double>("<xmlattr>.tolerance");
    return ex;
}

source_file parse_file(const ptree& node)
{
    source_file file;
    file.name = node.get<std::string>("<xmlattr>.name");
    return file;
}

void parse_source_files(const ptree& node, source_files& files)
{
    for (const ptree::value_type& v : node) {
        if (v.first == "File") {
            auto file = parse_file(v.second);
            files.push_back(file);
        }
    }
}

void parse_unknown_dependencies(const std::string& str, std::vector<unsigned int>& store)
{
    unsigned int i;
    std::stringstream ss(str);
    while (ss >> i) {
        store.push_back(i);
        if (ss.peek() == ',' || ss.peek() == ' ') {
            ss.ignore();
        }
    }
}

void parse_unknown_dependencies_kind(const std::string& str, std::vector<std::string>& store)
{
    boost::split(store, str, [](char c) { return c == ' '; });
}

unknown parse_unknown(const ptree& node)
{

    unknown unknown;
    unknown.index = node.get<unsigned int>("<xmlattr>.index");

    auto optDependencies = node.get_optional<std::string>("<xmlattr>.dependencies");
    if (optDependencies) {
        std::vector<unsigned int> dependencies;
        parse_unknown_dependencies(*optDependencies, dependencies);
        unknown.dependencies = dependencies;
    }

    auto optDependenciesKind = node.get_optional<std::string>("<xmlattr>.dependenciesKind");
    if (optDependenciesKind) {
        std::vector<std::string> dependenciesKind;
        parse_unknown_dependencies_kind(*optDependenciesKind, dependenciesKind);
        unknown.dependencies_kind = dependenciesKind;
    }

    return unknown;
}

void load_unknowns(const ptree& node, std::vector<unknown>& vector)
{
    for (const ptree::value_type& v : node) {
        if (v.first == "Unknown") {
            auto unknown = parse_unknown(v.second);
            vector.push_back(unknown);
        }
    }
}

std::unique_ptr<const model_structure> parse_model_structure(const ptree& node)
{

    std::vector<unknown> outputs;
    std::vector<unknown> derivatives;
    std::vector<unknown> initial_unknowns;

    for (const ptree::value_type& v : node) {
        if (v.first == "Outputs") {
            load_unknowns(v.second, outputs);
        } else if (v.first == "Derivatives") {
            load_unknowns(v.second, derivatives);
        } else if (v.first == "InitialUnknowns") {
            load_unknowns(v.second, initial_unknowns);
        }
    }

    return std::make_unique<const model_structure>(outputs, derivatives, initial_unknowns);
}

fmu_attributes parse_fmu_attributes(const ptree& node)
{

    fmu_attributes attributes;

    attributes.model_identifier = node.get<std::string>("<xmlattr>.modelIdentifier");
    attributes.needs_execution_tool = node.get<bool>("xmlattr>.needsExecutionTool", false);
    attributes.can_get_and_set_fmu_state = node.get<bool>("xmlattr>.canGetAndSetFMUstate", false);
    attributes.can_serialize_fmu_state = node.get<bool>("xmlattr>.canSerializeFMUstate", false);
    attributes.provides_directional_derivative = node.get<bool>("xmlattr>.providesDirectionalDerivative", false);
    attributes.can_not_use_memory_management_functions = node.get<bool>("xmlattr>.canNotUseMemoryManagementFunctions",
        false);
    attributes.can_be_instantiated_only_once_per_process = node.get<bool>("xmlattr>.canBeInstantiatedOnlyOncePerProcess",
        false);

    for (const ptree::value_type& v : node) {
        if (v.first == "SourceFiles") {
            parse_source_files(v.second, attributes.sourceFiles);
        }
    }

    return attributes;
}

cs_attributes parse_cs_attributes(const ptree& node)
{
    cs_attributes attributes(parse_fmu_attributes(node));
    attributes.max_output_derivative_order = node.get<unsigned int>("<xmlattr>.maxOutputDerivativeOrder", 0);
    attributes.can_interpolate_inputs = node.get<bool>("<xmlattr>.canInterpolateInputs", false);
    attributes.can_run_asynchronuously = node.get<bool>("<xmlattr>.canRunAsynchronuously", false);
    attributes.can_handle_variable_communication_step_size = node.get<bool>(
        "<xmlattr>.canHandleVariableCommunicationStepSize", false);
    return attributes;
}

me_attributes parse_me_attributes(const ptree& node)
{
    me_attributes attributes(parse_fmu_attributes(node));
    attributes.completed_integrator_step_not_needed = node.get<bool>(
        "<xmlattr>.completedIntegratorStepNotNeeded", false);
    return attributes;
}

template<typename T>
scalar_variable_attribute<T> parse_scalar_variable_attributes(const ptree& node)
{
    scalar_variable_attribute<T> attributes;
    attributes.start = node.get_optional<T>("<xmlattr>.start");
    attributes.declared_type = node.get_optional<std::string>("<xmlattr>.declaredType");
    return attributes;
}

template<typename T>
bounded_scalar_variable_attribute<T> parse_bounded_scalar_variable_attributes(const ptree& node)
{
    bounded_scalar_variable_attribute<T> attributes(parse_scalar_variable_attributes<T>(node));
    attributes.min = node.get_optional<T>("<xmlattr>.min");
    attributes.max = node.get_optional<T>("<xmlattr>.max");
    attributes.quantity = node.get_optional<std::string>("<xmlattr>.quantity");
    return attributes;
}

integer_attribute parse_integer_attribute(const ptree& node)
{
    return integer_attribute(parse_bounded_scalar_variable_attributes<int>(node));
}

real_attribute parse_real_attribute(const ptree& node)
{
    real_attribute attributes(parse_bounded_scalar_variable_attributes<double>(node));
    attributes.nominal = node.get_optional<double>("<xmlattr>.nominal");
    attributes.unit = node.get_optional<std::string>("<xmlattr>.unit");
    attributes.derivative = node.get_optional<unsigned int>("<xmlattr>.derivative");
    attributes.reinit = node.get<bool>("<xmlattr>.reinit", false);
    attributes.unbounded = node.get<bool>("<xmlattr>.unbounded", false);
    attributes.relative_quantity = node.get<bool>("<xmlattr>.relativeQuantity", false);
    return attributes;
}

string_attribute parse_string_attribute(const ptree& node)
{
    return string_attribute(parse_scalar_variable_attributes<std::string>(node));
}

boolean_attribute parse_boolean_attribute(const ptree& node)
{
    return boolean_attribute(parse_scalar_variable_attributes<bool>(node));
}

enumeration_attribute parseEnumerationAttribute(const ptree& node)
{
    return enumeration_attribute(parse_bounded_scalar_variable_attributes<int>(node));
}


scalar_variable parse_scalar_variable(const ptree& node)
{
    scalar_variable_base base;

    base.name = node.get<std::string>("<xmlattr>.name");
    base.description = node.get<std::string>("<xmlattr>.description", "");
    base.value_reference = node.get<fmi2ValueReference>("<xmlattr>.valueReference");
    base.can_handle_multiple_set_per_time_instant = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

    base.causality = parse_causality(node.get<std::string>("<xmlattr>.causality", ""));
    base.variability = parse_variability(node.get<std::string>("<xmlattr>.variability", ""));
    base.initial = parse_initial(node.get<std::string>("<xmlattr>.initial", ""));

    for (const ptree::value_type& v : node) {
        if (v.first == INTEGER_TYPE) {
            return scalar_variable(base, parse_integer_attribute(v.second));
        } else if (v.first == REAL_TYPE) {
            return scalar_variable(base, parse_real_attribute(v.second));
        } else if (v.first == STRING_TYPE) {
            return scalar_variable(base, parse_string_attribute(v.second));
        } else if (v.first == BOOLEAN_TYPE) {
            return scalar_variable(base, parse_boolean_attribute(v.second));
        } else if (v.first == ENUMERATION_TYPE) {
            return scalar_variable(base, parseEnumerationAttribute(v.second));
        }
    }

    throw std::runtime_error("FATAL: Failed to parse ScalarVariable!");
}

std::unique_ptr<const model_variables> parse_model_variables(const ptree& node)
{
    std::vector<scalar_variable> variables;
    for (const ptree::value_type& v : node) {
        if (v.first == "ScalarVariable") {
            auto var = parse_scalar_variable(v.second);
            variables.push_back(var);
        }
    }
    return std::make_unique<const model_variables>(variables);
}

} // namespace

std::unique_ptr<const model_description> fmi4cpp::fmi2::parse_model_description(const std::string& fileName)
{

    ptree tree;
    read_xml(fileName, tree);
    ptree root = tree.get_child("fmiModelDescription");

    model_description_base base;

    base.guid = root.get<std::string>("<xmlattr>.guid");
    base.fmi_version = root.get<std::string>("<xmlattr>.fmiVersion");
    base.model_name = root.get<std::string>("<xmlattr>.modelName");
    base.description = root.get<std::string>("<xmlattr>.description", "");
    base.author = root.get<std::string>("<xmlattr>.author", "");
    base.version = root.get<std::string>("<xmlattr>.version", "");
    base.license = root.get<std::string>("<xmlattr>.license", "");
    base.copyright = root.get<std::string>("<xmlattr>.copyright", "");
    base.generation_tool = root.get<std::string>("<xmlattr>.generationTool", "");
    base.generation_date_and_time = root.get<std::string>("<xmlattr>.generationDateAndTime", "");
    base.number_of_event_indicators = root.get<size_t>("<xmlattr>.numberOfEventIndicators", 0);
    base.variable_naming_convention = root.get<std::string>("<xmlattr>.variableNamingConvention",
        DEFAULT_VARIABLE_NAMING_CONVENTION);

    boost::optional<cs_attributes> coSimulation;
    boost::optional<me_attributes> modelExchange;

    for (const ptree::value_type& v : root) {

        if (v.first == "CoSimulation") {
            coSimulation = parse_cs_attributes(v.second);
        } else if (v.first == "ModelExchange") {
            modelExchange = parse_me_attributes(v.second);
        } else if (v.first == "DefaultExperiment") {
            base.default_experiment = parse_default_experiment(v.second);
        } else if (v.first == "ModelVariables") {
            base.model_variables = std::move(parse_model_variables(v.second));
        } else if (v.first == "ModelStructure") {
            base.model_structure = std::move(parse_model_structure(v.second));
        }
    }

    return std::make_unique<const model_description>(base, coSimulation, modelExchange);
}

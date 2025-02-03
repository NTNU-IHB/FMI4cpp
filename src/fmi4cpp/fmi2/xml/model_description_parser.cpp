
#include <fmi4cpp/fmi2/xml/model_description_parser.hpp>
#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>

#include <pugixml.hpp>
#include <sstream>
#include <stdexcept>

using namespace fmi4cpp::fmi2;

namespace
{

void split(std::vector<std::string>& store, const std::string& target, char c)
{
    std::string temp;
    std::stringstream ss{target};
    std::vector<std::string> result;

    while (std::getline(ss, temp, c)) {
        store.push_back(temp);
    }
}


void parse_unknown_dependencies_kind(const std::string& str, std::vector<std::string>& store)
{
    split(store, str, ' ');
}

template<typename T>
T parse_attribute(const pugi::xml_node& node, const std::string& name)
{
    if constexpr (std::is_same_v<T, int>) {
        return node.attribute(name.c_str()).as_int();
    } else if constexpr (std::is_same_v<T, unsigned int>) {
        return node.attribute(name.c_str()).as_uint();
    } else if constexpr (std::is_same_v<T, double>) {
        return node.attribute(name.c_str()).as_double();
    } else if constexpr (std::is_same_v<T, bool>) {
        return node.attribute(name.c_str()).as_bool();
    } else if constexpr (std::is_same_v<T, std::string>) {
        return node.attribute(name.c_str()).as_string();
    } else {
        throw std::runtime_error("Unable to parse attribute: " + name);
    }
}

template<typename T>
std::optional<T> parse_optional_attribute(const pugi::xml_node& node, const std::string& name)
{
    if (node.attribute(name.c_str()).empty()) {
        return std::nullopt;
    }
    return parse_attribute<T>(node, name);
}


default_experiment parse_default_experiment(const pugi::xml_node& node)
{
    default_experiment ex;
    ex.startTime = parse_optional_attribute<double>(node, "startTime");
    ex.stopTime = parse_optional_attribute<double>(node, "stopTime");
    ex.stepSize = parse_optional_attribute<double>(node, "stepSize");
    ex.tolerance = parse_optional_attribute<double>(node, "tolerance");
    return ex;
}

source_file parse_file(const pugi::xml_node& node)
{
    source_file file;
    file.name = node.attribute("name").as_string();
    return file;
}

void parse_source_files(const pugi::xml_node& node, source_files& files)
{
    for (const pugi::xml_node& v : node) {
        if (std::string("File") == v.name()) {
            auto file = parse_file(v);
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

unknown parse_unknown(const pugi::xml_node& node)
{

    unknown unknown;
    unknown.index = node.attribute("index").as_uint();

    auto optDependencies = parse_optional_attribute<std::string>(node, "dependencies");
    if (optDependencies) {
        std::vector<unsigned int> dependencies;
        parse_unknown_dependencies(*optDependencies, dependencies);
        unknown.dependencies = dependencies;
    }

    auto optDependenciesKind = parse_optional_attribute<std::string>(node, "dependenciesKind");
    if (optDependenciesKind) {
        std::vector<std::string> dependenciesKind;
        parse_unknown_dependencies_kind(*optDependenciesKind, dependenciesKind);
        unknown.dependencies_kind = dependenciesKind;
    }

    return unknown;
}

void load_unknowns(const pugi::xml_node& node, std::vector<unknown>& vector)
{
    for (const pugi::xml_node& v : node) {
        if (std::string("Unknown") == v.name()) {
            auto unknown = parse_unknown(v);
            vector.push_back(unknown);
        }
    }
}

std::unique_ptr<const model_structure> parse_model_structure(const pugi::xml_node& node)
{

    std::vector<unknown> outputs;
    std::vector<unknown> derivatives;
    std::vector<unknown> initial_unknowns;

    for (const pugi::xml_node& v : node) {
        if (std::string("Outputs") == v.name()) {
            load_unknowns(v, outputs);
        } else if (std::string("Derivatives") == v.name()) {
            load_unknowns(v, derivatives);
        } else if (std::string("InitialUnknowns") == v.name()) {
            load_unknowns(v, initial_unknowns);
        }
    }

    return std::make_unique<const model_structure>(outputs, derivatives, initial_unknowns);
}

base_unit parse_base_unit(const pugi::xml_node& node)
{
    base_unit baseUnit;
    baseUnit.kg = parse_attribute<int>(node, "kg");
    baseUnit.m = parse_attribute<int>(node, "m");
    baseUnit.s = parse_attribute<int>(node, "s");
    baseUnit.A = parse_attribute<int>(node, "A");
    baseUnit.K = parse_attribute<int>(node, "K");
    baseUnit.mol = parse_attribute<int>(node, "mol");
    baseUnit.cd = parse_attribute<int>(node, "cd");
    baseUnit.rad = parse_attribute<int>(node, "rad");
    baseUnit.factor = parse_attribute<double>(node, "factor");
    baseUnit.offset = parse_attribute<double>(node, "offset");

    return baseUnit;
}

display_unit parse_display_unit(const pugi::xml_node& node)
{
    display_unit displayUnit;
    displayUnit.name = parse_attribute<std::string>(node, "name");
    displayUnit.factor = parse_attribute<double>(node, "factor");
    displayUnit.offset = parse_attribute<double>(node, "offset");

    return displayUnit;
}

unit parse_unit(const pugi::xml_node& node)
{
    unit Unit;
    for (const pugi::xml_node& v : node) {
        if (std::string("BaseUnit") == v.name()) {
            Unit.base_unit = parse_base_unit(v);
        } else if (std::string("DisplayUnit") == v.name()) {
            Unit.display_units.push_back(parse_display_unit(v));
        }
    }
    Unit.name = parse_attribute<std::string>(node, "name");

    return Unit;
}

unit_definitions parse_unit_definitions(const pugi::xml_node& node)
{
    unit_definitions unitDefs;
    for (const pugi::xml_node& v : node) {
        if (std::string("Unit") == v.name()) {
            unitDefs.units.push_back(parse_unit(v));
        }
    }
    return unitDefs;
}

fmu_attributes parse_fmu_attributes(const pugi::xml_node& node)
{

    fmu_attributes attributes;

    attributes.model_identifier = node.attribute("modelIdentifier").as_string();
    attributes.needs_execution_tool = node.attribute("needsExecutionTool").as_bool();
    attributes.can_get_and_set_fmu_state = node.attribute("canGetAndSetFMUstate").as_bool();
    attributes.can_serialize_fmu_state = node.attribute("canSerializeFMUstate").as_bool();
    attributes.provides_directional_derivative = node.attribute("providesDirectionalDerivative").as_bool();
    attributes.can_not_use_memory_management_functions = node.attribute("canNotUseMemoryManagementFunctions").as_bool();
    attributes.can_be_instantiated_only_once_per_process = node.attribute("canBeInstantiatedOnlyOncePerProcess").as_bool();

    for (const pugi::xml_node& v : node) {
        if (std::string("SourceFiles") == v.name()) {
            parse_source_files(v, attributes.sourceFiles);
        }
    }

    return attributes;
}

cs_attributes parse_cs_attributes(const pugi::xml_node& node)
{
    cs_attributes attributes(parse_fmu_attributes(node));
    attributes.max_output_derivative_order = node.attribute("maxOutputDerivativeOrder").as_uint();
    attributes.can_interpolate_inputs = node.attribute("canInterpolateInputs").as_bool();
    attributes.can_run_asynchronuously = node.attribute("canRunAsynchronuously").as_bool();
    attributes.can_handle_variable_communication_step_size = node.attribute("canHandleVariableCommunicationStepSize").as_bool();
    return attributes;
}

me_attributes parse_me_attributes(const pugi::xml_node& node)
{
    me_attributes attributes(parse_fmu_attributes(node));
    attributes.completed_integrator_step_not_needed = node.attribute("completedIntegratorStepNotNeeded").as_bool();
    return attributes;
}

template<typename T>
scalar_variable_attribute<T> parse_scalar_variable_attributes(const pugi::xml_node& node)
{
    scalar_variable_attribute<T> attributes;
    attributes.start = parse_attribute<T>(node, "start");
    attributes.declared_type = parse_optional_attribute<std::string>(node, "declaredType");
    return attributes;
}

template<typename T>
bounded_scalar_variable_attribute<T> parse_bounded_scalar_variable_attributes(const pugi::xml_node& node)
{
    bounded_scalar_variable_attribute<T> attributes(parse_scalar_variable_attributes<T>(node));
    attributes.min = parse_optional_attribute<T>(node, "min");
    attributes.max = parse_optional_attribute<T>(node, "max");
    attributes.quantity = parse_optional_attribute<std::string>(node, "quantity");
    return attributes;
}

integer_attribute parse_integer_attribute(const pugi::xml_node& node)
{
    return integer_attribute(parse_bounded_scalar_variable_attributes<int>(node));
}

real_attribute parse_real_attribute(const pugi::xml_node& node)
{
    real_attribute attributes(parse_bounded_scalar_variable_attributes<double>(node));
    attributes.nominal = parse_optional_attribute<double>(node, "nominal");
    attributes.unit = parse_optional_attribute<std::string>(node, "unit");
    attributes.derivative = parse_optional_attribute<unsigned int>(node, "derivative");
    attributes.reinit = node.attribute("reinit").as_bool();
    attributes.unbounded = node.attribute("<unbounded").as_bool();
    attributes.relative_quantity = node.attribute("relativeQuantity").as_bool();
    return attributes;
}

string_attribute parse_string_attribute(const pugi::xml_node& node)
{
    return string_attribute(parse_scalar_variable_attributes<std::string>(node));
}

boolean_attribute parse_boolean_attribute(const pugi::xml_node& node)
{
    return boolean_attribute(parse_scalar_variable_attributes<bool>(node));
}

enumeration_attribute parseEnumerationAttribute(const pugi::xml_node& node)
{
    return enumeration_attribute(parse_bounded_scalar_variable_attributes<int>(node));
}


scalar_variable parse_scalar_variable(const pugi::xml_node& node)
{
    scalar_variable_base base;

    base.name = node.attribute("name").as_string();
    base.description = node.attribute("description").as_string();
    base.value_reference = node.attribute("valueReference").as_uint();
    base.can_handle_multiple_set_per_time_instant = node.attribute("canHandleMultipleSetPerTimelnstant").as_bool();

    base.causality = parse_causality(node.attribute("causality").as_string());
    base.variability = parse_variability(node.attribute("variability").as_string());
    base.initial = parse_initial(node.attribute("initial").as_string());

    for (const pugi::xml_node& v : node) {
        if (INTEGER_TYPE == v.name()) {
            return {base, parse_integer_attribute(v)};
        } else if (REAL_TYPE == v.name()) {
            return {base, parse_real_attribute(v)};
        } else if (STRING_TYPE == v.name()) {
            return {base, parse_string_attribute(v)};
        } else if (BOOLEAN_TYPE == v.name()) {
            return {base, parse_boolean_attribute(v)};
        } else if (ENUMERATION_TYPE == v.name()) {
            return {base, parseEnumerationAttribute(v)};
        }
    }

    throw std::runtime_error("FATAL: Failed to parse ScalarVariable!");
}

std::unique_ptr<const model_variables> parse_model_variables(const pugi::xml_node& node)
{
    std::vector<scalar_variable> variables;
    for (const pugi::xml_node& v : node) {
        if (std::string("ScalarVariable") == v.name()) {
            auto var = parse_scalar_variable(v);
            variables.push_back(var);
        }
    }
    return std::make_unique<const model_variables>(variables);
}

} // namespace

std::unique_ptr<const model_description> fmi4cpp::fmi2::parse_model_description(const std::string& fileName)
{

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());
    if (!result) {
        throw std::runtime_error("Unable to parse modelDescription.xml");
    }
    auto root = doc.child("fmiModelDescription");

    model_description_base base;

    base.guid = root.attribute("guid").as_string();
    base.fmi_version = root.attribute("fmiVersion").as_string();
    base.model_name = root.attribute("modelName").as_string();
    base.description = root.attribute("description").as_string();
    base.author = root.attribute("author").as_string();
    base.version = root.attribute("version").as_string();
    base.license = root.attribute("license").as_string();
    base.copyright = root.attribute("copyright").as_string();
    base.generation_tool = root.attribute("generationTool").as_string();
    base.generation_date_and_time = root.attribute("generationDateAndTime").as_string();
    base.number_of_event_indicators = root.attribute("numberOfEventIndicators").as_ullong();
    base.variable_naming_convention = root.attribute("variableNamingConvention").as_string("flat");

    std::optional<cs_attributes> coSimulation;
    std::optional<me_attributes> modelExchange;

    for (const auto& v : root) {
        if (std::string("CoSimulation") == v.name()) {
            coSimulation = parse_cs_attributes(v);
        } else if (std::string("ModelExchange") == v.name()) {
            modelExchange = parse_me_attributes(v);
        } else if (std::string("DefaultExperiment") == v.name()) {
            base.default_experiment = parse_default_experiment(v);
        } else if (std::string("ModelVariables") == v.name()) {
            base.model_variables = std::move(parse_model_variables(v));
        } else if (std::string("ModelStructure") == v.name()) {
            base.model_structure = std::move(parse_model_structure(v));
        } else if (std::string("UnitDefinitions") == v.name()) {
            base.unit_definitions = std::move(parse_unit_definitions(v));
        }
    }

    return std::make_unique<const model_description>(base, coSimulation, modelExchange);
}

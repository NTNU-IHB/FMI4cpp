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


#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>
#include <fmi4cpp/fmi2/xml/model_description_parser.hpp>

#include <fmi4cpp/tools/optional_converter.hpp>

using boost::property_tree::ptree;
using namespace fmi4cpp::fmi2;

namespace {

    const char* DEFAULT_VARIABLE_NAMING_CONVENTION = "flat";

    const default_experiment parseDefaultExperiment(const ptree &node) {
        default_experiment ex;
        ex.startTime = convert(node.get_optional<double>("<xmlattr>.startTime"));
        ex.stopTime = convert(node.get_optional<double>("<xmlattr>.stopTime"));
        ex.stepSize = convert(node.get_optional<double>("<xmlattr>.stepSize"));
        ex.tolerance = convert(node.get_optional<double>("<xmlattr>.tolerance"));
        return ex;
    }

    const source_file parseFile(const ptree &node) {
        source_file file;
        file.name = node.get<std::string>("<xmlattr>.name");
        return file;
    }

    void parseSourceFiles(const ptree &node, source_files &files) {
        for (const ptree::value_type &v : node) {
            if (v.first == "File") {
                auto file = parseFile(v.second);
                files.push_back(file);
            }
        }
    }

    void parseUnknownDependencies(const std::string &str, std::vector<unsigned int> &store) {
        unsigned int i;
        std::stringstream ss(str);
        while (ss >> i) {
            store.push_back(i);
            if (ss.peek() == ',' || ss.peek() == ' ') {
                ss.ignore();
            }
        }
    }

    void parseUnknownDependenciesKind(const std::string &str, std::vector<std::string> &store) {
        boost::split(store, str, [](char c) { return c == ' '; });
    }

    const unknown parseUnknown(const ptree &node) {

        unknown unknown;
        unknown.index = node.get<unsigned int>("<xmlattr>.index");

        auto opt_dependencies = node.get_optional<std::string>("<xmlattr>.dependencies");
        if (opt_dependencies) {
            std::vector<unsigned int> dependencies;
            parseUnknownDependencies(*opt_dependencies, dependencies);
            unknown.dependencies = dependencies;
        }

        auto opt_dependenciesKind = node.get_optional<std::string>("<xmlattr>.dependenciesKind");
        if (opt_dependenciesKind) {
            std::vector<std::string> dependenciesKind;
            parseUnknownDependenciesKind(*opt_dependenciesKind, dependenciesKind);
            unknown.dependenciesKind = dependenciesKind;
        }

        return unknown;
    }

    void loadUnknowns(const ptree &node, std::vector<unknown> &vector) {
        for (const ptree::value_type &v : node) {
            if (v.first == "Unknown") {
                auto unknown = parseUnknown(v.second);
                vector.push_back(unknown);
            }
        }
    }

    std::unique_ptr<const model_structure> parse_model_structure(const ptree &node) {

        std::vector<unknown> outputs;
        std::vector<unknown> derivatives;
        std::vector<unknown> initialUnknowns;

        for (const ptree::value_type &v : node) {
            if (v.first == "Outputs") {
                loadUnknowns(v.second, outputs);
            } else if (v.first == "Derivatives") {
                loadUnknowns(v.second, derivatives);
            } else if (v.first == "InitialUnknowns") {
                loadUnknowns(v.second, initialUnknowns);
            }
        }

        return std::make_unique<const model_structure>(outputs, derivatives, initialUnknowns);

    }

    fmu_attributes parse_fmu_attributes(const ptree &node) {

        fmu_attributes attributes;

        attributes.modelIdentifier = node.get<std::string>("<xmlattr>.modelIdentifier");
        attributes.needsExecutionTool = node.get<bool>("xmlattr>.needsExecutionTool", false);
        attributes.canGetAndSetFMUstate = node.get<bool>("xmlattr>.canGetAndSetFMUstate", false);
        attributes.canSerializeFMUstate = node.get<bool>("xmlattr>.canSerializeFMUstate", false);
        attributes.providesDirectionalDerivative = node.get<bool>("xmlattr>.providesDirectionalDerivative", false);
        attributes.canNotUseMemoryManagementFunctions = node.get<bool>("xmlattr>.canNotUseMemoryManagementFunctions",
                                                                       false);
        attributes.canBeInstantiatedOnlyOncePerProcess = node.get<bool>("xmlattr>.canBeInstantiatedOnlyOncePerProcess",
                                                                        false);

        for (const ptree::value_type &v : node) {
            if (v.first == "SourceFiles") {
                parseSourceFiles(v.second, attributes.sourceFiles);
            }
        }

        return attributes;

    }

    const cs_attributes parse_cs_attributes(const ptree &node) {

        cs_attributes attributes(parse_fmu_attributes(node));
        attributes.maxOutputDerivativeOrder = node.get<unsigned int>("<xmlattr>.maxOutputDerivativeOrder", 0);
        attributes.canInterpolateInputs = node.get<bool>("<xmlattr>.canInterpolateInputs", false);
        attributes.canRunAsynchronuously = node.get<bool>("<xmlattr>.canRunAsynchronuously", false);
        attributes.canHandleVariableCommunicationStepSize = node.get<bool>(
                "<xmlattr>.canHandleVariableCommunicationStepSize", false);
        return attributes;

    }

    const me_attributes parse_me_attributes(const ptree &node) {
        me_attributes attributes(parse_fmu_attributes(node));
        attributes.completedIntegratorStepNotNeeded = node.get<bool>(
                "<xmlattr>.completedIntegratorStepNotNeeded", false);
        return attributes;
    }

    template<typename T>
    scalar_variable_attribute<T> parse_scalar_variable_attributes(const ptree &node) {
        scalar_variable_attribute<T> attributes;
        attributes.start = convert(node.get_optional<T>("<xmlattr>.start"));
        attributes.declared_type = convert(node.get_optional<std::string>("<xmlattr>.declaredType"));
        return attributes;
    }

    template<typename T>
    bounded_scalar_variable_attribute<T> parse_bounded_scalar_variable_attributes(const ptree &node) {
        bounded_scalar_variable_attribute<T> attributes(parse_scalar_variable_attributes<T>(node));
        attributes.min = convert(node.get_optional<T>("<xmlattr>.min"));
        attributes.max = convert(node.get_optional<T>("<xmlattr>.max"));
        attributes.quantity = convert(node.get_optional<std::string>("<xmlattr>.quantity"));
        return attributes;
    }

    integer_attribute parse_integer_attribute(const ptree &node) {
        return integer_attribute(parse_bounded_scalar_variable_attributes<int>(node));
    }

    real_attribute parse_real_attribute(const ptree &node) {
        real_attribute attributes(parse_bounded_scalar_variable_attributes<double>(node));
        attributes.nominal = convert(node.get_optional<double>("<xmlattr>.nominal"));
        attributes.unit = convert(node.get_optional<std::string>("<xmlattr>.unit"));
        attributes.derivative = convert(node.get_optional<unsigned int>("<xmlattr>.derivative"));
        attributes.reinit = node.get<bool>("<xmlattr>.reinit", false);
        attributes.unbounded = node.get<bool>("<xmlattr>.unbounded", false);
        attributes.relativeQuantity = node.get<bool>("<xmlattr>.relativeQuantity", false);
        return attributes;
    }

    string_attribute parse_string_attribute(const ptree &node) {
        return string_attribute(parse_scalar_variable_attributes<std::string>(node));
    }

    boolean_attribute parse_boolean_attribute(const ptree &node) {
        return boolean_attribute(parse_scalar_variable_attributes<bool>(node));
    }

    enumeration_attribute parseEnumerationAttribute(const ptree &node) {
        return enumeration_attribute(parse_bounded_scalar_variable_attributes<int>(node));
    }


    const scalar_variable parse_scalar_variable(const ptree &node) {

        scalar_variable_base base;

        base.name = node.get<std::string>("<xmlattr>.name");
        base.description = node.get<std::string>("<xmlattr>.description", "");
        base.valueReference = node.get<fmi2ValueReference>("<xmlattr>.valueReference");
        base.canHandleMultipleSetPerTimelnstant = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

        base.causality = parse_causality(node.get<std::string>("<xmlattr>.causality", ""));
        base.variability = parse_variability(node.get<std::string>("<xmlattr>.variability", ""));
        base.initial = parse_initial(node.get<std::string>("<xmlattr>.initial", ""));

        for (const ptree::value_type &v : node) {
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

    std::unique_ptr<const model_variables> parse_model_variables(const ptree &node) {
        std::vector<scalar_variable> variables;
        for (const ptree::value_type &v : node) {
            if (v.first == "ScalarVariable") {
                auto var = parse_scalar_variable(v.second);
                variables.push_back(var);
            }
        }
        return std::make_unique<const model_variables>(variables);
    }

}

std::unique_ptr<const model_description> fmi4cpp::fmi2::parse_model_description(const std::string &fileName) {

    ptree tree;
    read_xml(fileName, tree);
    ptree root = tree.get_child("fmiModelDescription");

    model_description_base base;

    base.guid = root.get<std::string>("<xmlattr>.guid");
    base.fmiVersion = root.get<std::string>("<xmlattr>.fmiVersion");
    base.modelName = root.get<std::string>("<xmlattr>.modelName");
    base.description = root.get<std::string>("<xmlattr>.description", "");
    base.author = root.get<std::string>("<xmlattr>.author", "");
    base.version = root.get<std::string>("<xmlattr>.version", "");
    base.license = root.get<std::string>("<xmlattr>.license", "");
    base.copyright = root.get<std::string>("<xmlattr>.copyright", "");
    base.generationTool = root.get<std::string>("<xmlattr>.generationTool", "");
    base.generationDateAndTime = root.get<std::string>("<xmlattr>.generationDateAndTime", "");
    base.numberOfEventIndicators = root.get<size_t>("<xmlattr>.numberOfEventIndicators", 0);
    base.variableNamingConvention = root.get<std::string>("<xmlattr>.variableNamingConvention",
                                                          DEFAULT_VARIABLE_NAMING_CONVENTION);

    std::optional<cs_attributes> coSimulation;
    std::optional<me_attributes> modelExchange;

    for (const ptree::value_type &v : root) {

        if (v.first == "CoSimulation") {
            coSimulation = parse_cs_attributes(v.second);
        } else if (v.first == "ModelExchange") {
            modelExchange = parse_me_attributes(v.second);
        } else if (v.first == "DefaultExperiment") {
            base.defaultExperiment = parseDefaultExperiment(v.second);
        } else if (v.first == "ModelVariables") {
            base.modelVariables = std::move(parse_model_variables(v.second));
        } else if (v.first == "ModelStructure") {
            base.modelStructure = std::move(parse_model_structure(v.second));
        }

    }

    return std::make_unique<const model_description>(base, coSimulation, modelExchange);

}


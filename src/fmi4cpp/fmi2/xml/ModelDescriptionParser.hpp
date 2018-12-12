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

#ifndef FMI4CPP_MODELDESCRIPTIONPARSER_HPP
#define FMI4CPP_MODELDESCRIPTIONPARSER_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <fmi4cpp/fmi2/xml/ModelDescription.hpp>
#include <fmi4cpp/fmi2/xml/ScalarVariableAttribute.hpp>

#include "../../tools/optional_converter.hpp"

using boost::property_tree::ptree;
using namespace fmi4cpp::fmi2;

namespace {

    const std::string DEFAULT_VARIABLE_NAMING_CONVENTION = "flat";

    DefaultExperiment parseDefaultExperiment(const ptree &node) {
        return DefaultExperiment(
                convert(node.get_optional<double>("<xmlattr>.startTime")),
                convert(node.get_optional<double>("<xmlattr>.stopTime")),
                convert(node.get_optional<double>("<xmlattr>.stepSize")),
                convert(node.get_optional<double>("<xmlattr>.tolerance")));
    }


    SourceFile parseFile(const ptree &node) {
        std::string name = node.get<std::string>("<xmlattr>.name");
        return SourceFile(name);
    }

    void parseSourceFiles(const ptree &node, SourceFiles &files) {
        for (const ptree::value_type &v : node) {
            if (v.first == "File") {
                SourceFile file = parseFile(v.second);
                files.push_back(file);
            }
        }
    }

    void parseUnknownDependencies(const std::string &str, std::vector<unsigned int> &store) {
        int i;
        std::stringstream ss(str);
        while (ss >> i) {
            store.push_back(i);
            if (ss.peek() == ',' || ss.peek() == ' ') {
                ss.ignore();
            }
        }
    }

    fmi4cpp::fmi2::Unknown parseUnknown(const ptree &node) {
        auto index = node.get<unsigned int>("<xmlattr>.index");
        auto dependenciesKind = convert(node.get_optional<std::string>("<xmlattr>.dependenciesKind"));

        std::vector<unsigned int> dependencies;
        auto opt_dependencies = node.get_optional<std::string>("<xmlattr>.dependencies");
        if (opt_dependencies) {
            parseUnknownDependencies(*opt_dependencies, dependencies);
        }
        return fmi4cpp::fmi2::Unknown(index, dependenciesKind, dependencies);
    }

    void loadUnknowns(const ptree &node, std::vector<fmi4cpp::fmi2::Unknown> &vector) {

        for (const ptree::value_type &v : node) {
            if (v.first == "Unknown") {
                fmi4cpp::fmi2::Unknown unknown = parseUnknown(v.second);
                vector.push_back(unknown);
            }
        }

    }

    std::unique_ptr<ModelStructure> parseModelStructure(const ptree &node) {

        std::vector<fmi4cpp::fmi2::Unknown> outputs;
        std::vector<fmi4cpp::fmi2::Unknown> derivatives;
        std::vector<fmi4cpp::fmi2::Unknown> initialUnknowns;

        for (const ptree::value_type &v : node) {
            if (v.first == "Outputs") {
                loadUnknowns(v.second, outputs);
            } else if (v.first == "Derivatives") {
                loadUnknowns(v.second, derivatives);
            } else if (v.first == "InitialUnknowns") {
                loadUnknowns(v.second, initialUnknowns);
            }
        }

        return std::make_unique<ModelStructure>(outputs, derivatives, initialUnknowns);

    }

    FmuAttributes parseFmuAttributes(const ptree &node) {

        auto modelIdentifier = node.get<std::string>("<xmlattr>.modelIdentifier");

        auto needsExecutionTool = node.get<bool>("xmlattr>.needsExecutionTool", false);
        auto canGetAndSetFMUstate = node.get<bool>("xmlattr>.canGetAndSetFMUstate", false);
        auto canSerializeFMUstate = node.get<bool>("xmlattr>.canSerializeFMUstate", false);
        auto providesDirectionalDerivative = node.get<bool>("xmlattr>.providesDirectionalDerivative", false);
        auto canNotUseMemoryManagementFunctions = node.get<bool>("xmlattr>.canNotUseMemoryManagementFunctions", false);
        auto canBeInstantiatedOnlyOncePerProcess = node.get<bool>("xmlattr>.canBeInstantiatedOnlyOncePerProcess",
                                                                  false);
        SourceFiles sourceFiles;
        for (const ptree::value_type &v : node) {
            if (v.first == "SourceFiles") {
                parseSourceFiles(v.second, sourceFiles);
            }
        }

        return FmuAttributes(modelIdentifier, canGetAndSetFMUstate, canSerializeFMUstate, needsExecutionTool,
                             canNotUseMemoryManagementFunctions, canBeInstantiatedOnlyOncePerProcess,
                             providesDirectionalDerivative, sourceFiles);

    }

    CoSimulationAttributes parseCoSimulationAttributes(const ptree &node) {

        auto commonAttributes = parseFmuAttributes(node);
        auto maxOutputDerivativeOrder = node.get<unsigned int>("<xmlattr>.maxOutputDerivativeOrder", 0);
        auto canInterpolateInputs = node.get<bool>("<xmlattr>.canInterpolateInputs", false);
        auto canRunAsynchronuously = node.get<bool>("<xmlattr>.canRunAsynchronuously", false);
        auto canHandleVariableCommunicationStepSize = node.get<bool>("<xmlattr>.canHandleVariableCommunicationStepSize",
                                                                     false);

        return CoSimulationAttributes(commonAttributes, canInterpolateInputs, canRunAsynchronuously,
                                      canHandleVariableCommunicationStepSize, maxOutputDerivativeOrder);

    }

    ModelExchangeAttributes parseModelExchangeAttributes(const ptree &node) {
        auto commonAttributes = parseFmuAttributes(node);
        auto completedIntegratorStepNotNeeded = node.get<bool>("<xmlattr>.completedIntegratorStepNotNeeded", false);
        return ModelExchangeAttributes(commonAttributes, completedIntegratorStepNotNeeded);
    }

    template<typename T>
    ScalarVariableAttribute<T> parseScalarVariableAttributes(const ptree &node) {
        ScalarVariableAttribute<T> attributes;
        attributes.start = convert(node.get_optional<T>("<xmlattr>.start"));
        attributes.declaredType = convert(node.get_optional<std::string>("<xmlattr>.declaredType"));
        return attributes;
    }

    template<typename T>
    BoundedScalarVariableAttribute<T> parseBoundedScalarVariableAttributes(const ptree &node) {
        BoundedScalarVariableAttribute<T> attributes(parseScalarVariableAttributes<T>(node));
        attributes.min = convert(node.get_optional<T>("<xmlattr>.min"));
        attributes.max = convert(node.get_optional<T>("<xmlattr>.max"));
        attributes.quantity = convert(node.get_optional<std::string>("<xmlattr>.quantity"));
        return attributes;
    }

    IntegerAttribute parseIntegerAttribute(const ptree &node) {
        return IntegerAttribute(parseBoundedScalarVariableAttributes<int>(node));
    }

    RealAttribute parseRealAttribute(const ptree &node) {
        RealAttribute attributes(parseBoundedScalarVariableAttributes<double>(node));
        attributes.nominal = convert(node.get_optional<double>("<xmlattr>.nominal"));
        attributes.unit = convert(node.get_optional<std::string>("<xmlattr>.unit"));
        attributes.derivative = convert(node.get_optional<unsigned int>("<xmlattr>.derivative"));
        attributes.reinit = node.get<bool>("<xmlattr>.reinit", false);
        attributes.unbounded = node.get<bool>("<xmlattr>.unbounded", false);
        attributes.relativeQuantity = node.get<bool>("<xmlattr>.relativeQuantity", false);
        return attributes;
    }

    StringAttribute parseStringAttribute(const ptree &node) {
        return StringAttribute(parseScalarVariableAttributes<std::string>(node));
    }

    BooleanAttribute parseBooleanAttribute(const ptree &node) {
        return BooleanAttribute(parseScalarVariableAttributes<bool>(node));
    }

    EnumerationAttribute parseEnumerationAttribute(const ptree &node) {
        return EnumerationAttribute(parseBoundedScalarVariableAttributes<int>(node));
    }


    ScalarVariable parseScalarVariable(const ptree &node) {

        auto name = node.get<std::string>("<xmlattr>.name");
        auto description = node.get<std::string>("<xmlattr>.description", "");
        auto valueReference = node.get<fmi2ValueReference>("<xmlattr>.valueReference");
        auto canHandleMultipleSetPerTimelnstant = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

        auto causality = parseCausality(node.get<std::string>("<xmlattr>.causality", ""));
        auto variability = parseVariability(node.get<std::string>("<xmlattr>.variability", ""));
        auto initial = parseInitial(node.get<std::string>("<xmlattr>.initial", ""));

        ScalarVariableBase base(name, description, valueReference, canHandleMultipleSetPerTimelnstant, causality,
                                variability, initial);

        for (const ptree::value_type &v : node) {
            if (v.first == INTEGER_TYPE) {
                return ScalarVariable(base, parseIntegerAttribute(v.second));
            } else if (v.first == REAL_TYPE) {
                return ScalarVariable(base, parseRealAttribute(v.second));
            } else if (v.first == STRING_TYPE) {
                return ScalarVariable(base, parseStringAttribute(v.second));
            } else if (v.first == BOOLEAN_TYPE) {
                return ScalarVariable(base, parseBooleanAttribute(v.second));
            } else if (v.first == ENUMERATION_TYPE) {
                return ScalarVariable(base, parseEnumerationAttribute(v.second));
            }
        }

        throw std::runtime_error("FATAL: Failed to parse ScalarVariable!");

    }

    std::unique_ptr<ModelVariables> parseModelVariables(const ptree &node) {
        std::vector<ScalarVariable> variables;
        for (const ptree::value_type &v : node) {
            if (v.first == "ScalarVariable") {
                auto var = parseScalarVariable(v.second);
                variables.push_back(var);
            }
        }
        return std::make_unique<ModelVariables>(variables);
    }

}


namespace fmi4cpp::fmi2 {

    std::unique_ptr<ModelDescription> parseModelDescription(const std::string &fileName) {

        ptree tree;
        read_xml(fileName, tree);
        ptree root = tree.get_child("fmiModelDescription");

        auto guid = root.get<std::string>("<xmlattr>.guid");
        auto fmiVersion = root.get<std::string>("<xmlattr>.fmiVersion");
        auto modelName = root.get<std::string>("<xmlattr>.modelName");
        auto description = root.get<std::string>("<xmlattr>.description", "");
        auto author = root.get<std::string>("<xmlattr>.author", "");
        auto version = root.get<std::string>("<xmlattr>.version", "");
        auto license = root.get<std::string>("<xmlattr>.license", "");
        auto copyright = root.get<std::string>("<xmlattr>.copyright", "");
        auto generationTool = root.get<std::string>("<xmlattr>.generationTool", "");
        auto generationDateAndTime = root.get<std::string>("<xmlattr>.generationDateAndTime", "");
        auto numberOfEventIndicators = root.get<size_t>("<xmlattr>.numberOfEventIndicators", 0);
        auto variableNamingConvention = root.get<std::string>("<xmlattr>.variableNamingConvention",
                                                              DEFAULT_VARIABLE_NAMING_CONVENTION);

        std::shared_ptr<ModelVariables> modelVariables;
        std::shared_ptr<ModelStructure> modelStructure;
        std::optional<DefaultExperiment> defaultExperiment;
        std::optional<CoSimulationAttributes> coSimulation;
        std::optional<ModelExchangeAttributes> modelExchange;

        for (const ptree::value_type &v : root) {

            if (v.first == "CoSimulation") {
                coSimulation = parseCoSimulationAttributes(v.second);
            } else if (v.first == "ModelExchange") {
                modelExchange = parseModelExchangeAttributes(v.second);
            } else if (v.first == "DefaultExperiment") {
                defaultExperiment = parseDefaultExperiment(v.second);
            } else if (v.first == "ModelVariables") {
                modelVariables = std::move(parseModelVariables(v.second));
            } else if (v.first == "ModelStructure") {
                modelStructure = std::move(parseModelStructure(v.second));
            }

        }

        const ModelDescriptionBase base(guid,
                                        fmiVersion,
                                        modelName,
                                        description,
                                        version,
                                        author,
                                        license,
                                        copyright,
                                        generationTool,
                                        generationDateAndTime,
                                        variableNamingConvention,
                                        numberOfEventIndicators,
                                        modelVariables,
                                        modelStructure,
                                        defaultExperiment);

        return std::make_unique<ModelDescription>(base, coSimulation, modelExchange);

    }

}

#endif //FMI4CPP_MODELDESCRIPTIONPARSER_HPP

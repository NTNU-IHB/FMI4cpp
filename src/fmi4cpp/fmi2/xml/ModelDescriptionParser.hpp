//
// Created by LarsIvar on 09.10.2018.
//

#ifndef FMI4CPP_MODELDESCRIPTIONPARSER_HPP
#define FMI4CPP_MODELDESCRIPTIONPARSER_HPP

#include <string>
#include <boost/property_tree/ptree.hpp>

#include <fmi4cpp/fmi2/xml/ModelDescription.hpp>

using boost::property_tree::ptree;

using namespace fmi4cpp::fmi2::xml;

namespace {

    template<class T>
    std::optional<T> convert(boost::optional<T> opt) {
        if (!opt) {
            return {};
        } else {
            return *opt;
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

    Unknown parseUnknown(const ptree &node) {
        auto index = node.get<unsigned int>("<xmlattr>.index");
        auto dependenciesKind = convert(node.get_optional<std::string>("<xmlattr>.dependenciesKind"));

        std::vector<unsigned int> dependencies;
        auto opt_dependencies = node.get_optional<std::string>("<xmlattr>.dependencies");
        if (opt_dependencies) {
            parseUnknownDependencies(*opt_dependencies, dependencies);
        }
        return Unknown(index, dependenciesKind, dependencies);
    }

    void loadUnknowns(const ptree &node, std::vector<Unknown> &vector) {

        for (const ptree::value_type &v : node) {
            if (v.first == "Unknown") {
                Unknown unknown = parseUnknown(v.second);
                vector.push_back(unknown);
            }
        }

    }

    DefaultExperiment parseDefaultExperiment(const ptree &node) {
        return DefaultExperiment(
                convert(node.get_optional<double>("<xmlattr>.startTime")),
                convert(node.get_optional<double>("<xmlattr>.stopTime")),
                convert(node.get_optional<double>("<xmlattr>.stepSize")),
                convert(node.get_optional<double>("<xmlattr>.tolerance")));
    }


    File parseFile(const ptree &node) {
        std::string name = node.get<std::string>("<xmlattr>.name");
        return File(name);
    }

    void parseSourceFiles(const ptree &node, SourceFiles &files) {
        for (const ptree::value_type &v : node) {
            if (v.first == "File") {
                File file = parseFile(v.second);
                files.push_back(file);
            }
        }
    }

    ModelStructure parseModelstructure(const ptree &node) {

        std::vector<Unknown> outputs;
        std::vector<Unknown> derivatives;
        std::vector<Unknown> initialUnknowns;

        for (const ptree::value_type &v : node) {
            if (v.first == "Outputs") {
                loadUnknowns(v.second, outputs);
            } else if (v.first == "Derivatives") {
                loadUnknowns(v.second, derivatives);
            } else if (v.first == "InitialUnknowns") {
                loadUnknowns(v.second, initialUnknowns);
            }
        }

        return ModelStructure(outputs, derivatives, initialUnknowns);

    }

    IntegerAttribute parseIntegerAttribute(const ptree &node) {
        auto min = convert(node.get_optional<int>("<xmlattr>.min"));
        auto max = convert(node.get_optional<int>("<xmlattr>.max"));
        auto start = convert(node.get_optional<int>("<xmlattr>.start"));
        auto declaredType = convert(node.get_optional<std::string>("<xmlattr>.declaredType"));
        auto quantity = convert(node.get_optional<std::string>("<xmlattr>.quantity"));

        return IntegerAttribute(start, declaredType, min, max, quantity);

    }

    RealAttribute parseRealAttribute(const ptree &node) {

        auto min = convert(node.get_optional<double>("<xmlattr>.min"));
        auto max = convert(node.get_optional<double>("<xmlattr>.max"));
        auto start = convert(node.get_optional<double>("<xmlattr>.start"));
        auto declaredType = convert(node.get_optional<std::string>("<xmlattr>.declaredType"));
        auto quantity = convert(node.get_optional<std::string>("<xmlattr>.quantity"));

        auto nominal = convert(node.get_optional<double>("<xmlattr>.nominal"));

        auto unit = convert(node.get_optional<std::string>("<xmlattr>.unit"));
        auto displayUnit = convert(node.get_optional<std::string>("<xmlattr>.displayUnit"));

        auto derivative = convert(node.get_optional<unsigned int>("<xmlattr>.derivative"));

        auto reinit = node.get<bool>("<xmlattr>.reinit", false);
        auto unbounded = node.get<bool>("<xmlattr>.unbounded", false);
        auto relativeQuantity = node.get<bool>("<xmlattr>.relativeQuantity", false);

        return RealAttribute(start, declaredType, min, max, quantity, reinit, unbounded, relativeQuantity, nominal,
                             derivative, unit, displayUnit);

    }

    StringAttribute parseStringAttribute(const ptree &node) {
        auto start = convert(node.get_optional<std::string>("<xmlattr>.start"));
        auto declaredType = convert(node.get_optional<std::string>("<xmlattr>.declaredType"));

        return StringAttribute(start, declaredType);
    }

    BooleanAttribute parseBooleanAttribute(const ptree &node) {
        auto start = convert(node.get_optional<bool>("<xmlattr>.start"));
        auto declaredType = convert(node.get_optional<std::string>("<xmlattr>.declaredType"));

        return BooleanAttribute(start, declaredType);
    }

    EnumerationAttribute parseEnumerationAttribute(const ptree &node) {
        auto min = convert(node.get_optional<double>("<xmlattr>.min"));
        auto max = convert(node.get_optional<double>("<xmlattr>.max"));
        auto start = convert(node.get_optional<double>("<xmlattr>.start"));
        auto declaredType = convert(node.get_optional<std::string>("<xmlattr>.declaredType"));
        auto quantity = convert(node.get_optional<std::string>("<xmlattr>.quantity"));

        return EnumerationAttribute(start, declaredType, min, max, quantity);
    }

    FmuAttributes parseFmuAttributes(const ptree &node) {

        auto modelIdentifier = node.get<std::string>("<xmlattr>.modelIdentifier");

        needsExecutionTool = node.get<bool>("xmlattr>.needsExecutionTool", false);
        canGetAndSetFMUstate = node.get<bool>("xmlattr>.canGetAndSetFMUstate", false);
        canSerializeFMUstate = node.get<bool>("xmlattr>.canSerializeFMUstate", false);
        providesDirectionalDerivative = node.get<bool>("xmlattr>.providesDirectionalDerivative", false);
        canNotUseMemoryManagementFunctions = node.get<bool>("xmlattr>.canNotUseMemoryManagementFunctions", false);
        canBeInstantiatedOnlyOncePerProcess = node.get<bool>("xmlattr>.canBeInstantiatedOnlyOncePerProcess", false);

        for (const ptree::value_type &v : node) {
            if (v.first == "SourceFiles") {

            }
        }

    }

}

namespace fmi4cpp::fmi2::xml {


    ModelDescription parse(std::string &fileName) {

        ptree tree;
        read_xml(fileName, tree);

        ptree root = tree.get_child("fmiModelDescription");
//
//        guid_ = root.get<string>("<xmlattr>.guid");
//        fmiVersion_ = root.get<string>("<xmlattr>.fmiVersion");
//        modelName_ = root.get<string>("<xmlattr>.modelName");
//        description_ = root.get<string>("<xmlattr>.description", "");
//        author_ = root.get<string>("<xmlattr>.author", "");
//        version_ = root.get<string>("<xmlattr>.version", "");
//        license_ = root.get<string>("<xmlattr>.license", "");
//        generationTool_ = root.get<string>("<xmlattr>.generationTool", "");
//        generationDateAndTime_ = root.get<string>("<xmlattr>.generationDateAndTime", "");
//        numberOfEventIndicators_ = root.get<unsigned int>("<xmlattr>.numberOfEventIndicators", 0);
//        variableNamingConvention_ = root.get<string>("<xmlattr>.variableNamingConvention", DEFAULT_VARIABLE_NAMING_CONVENTION);
//
//        for (const ptree::value_type &v : root) {
//
//            if (v.first == "CoSimulation") {
//                coSimulation_ = CoSimulationAttributes();
//                coSimulation_->load(v.second);
//            } else if (v.first == "ModelExchange") {
//                modelExchange_ = ModelExchangeAttributes();
//                modelExchange_->load(v.second);
//            } else if (v.first == "DefaultExperiment") {
//                defaultExperiment_ = DefaultExperiment();
//                defaultExperiment_->load(v.second);
//            } else if (v.first == "ModelVariables") {
//                modelVariables_ = make_shared<ModelVariablesImpl>(v.second);
//            } else if (v.first == "ModelStructure") {
//                modelStructure_ = make_shared<ModelStructureImpl>(v.second);
//            }
//
//        }
//
    }

}

#endif //FMI4CPP_MODELDESCRIPTIONPARSER_HPP

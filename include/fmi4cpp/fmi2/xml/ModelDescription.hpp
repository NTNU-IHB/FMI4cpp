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

#ifndef FMI4CPP_MODELDESCRIPTION_HPP
#define FMI4CPP_MODELDESCRIPTION_HPP

#include <memory>
#include <string>
#include <optional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "ModelStructure.hpp"
#include "ModelVariables.hpp"
#include "DefaultExperiment.hpp"
#include "FmuAttributes.hpp"

using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    class ModelDescriptionBase {

    private:
        std::string guid_;
        std::string fmiVersion_;
        std::string modelName_;
        std::optional<std::string> description_;
        std::optional<std::string> version_;
        std::optional<std::string> author_;
        std::optional<std::string> license_;
        std::optional<std::string> copyright_;
        std::optional<std::string> generationTool_;
        std::optional<std::string> generationDateAndTime_;
        std::optional<std::string> variableNamingConvention_;

        size_t numberOfEventIndicators_;

        ModelVariables modelVariables_;
        ModelStructure modelStructure_;

        std::optional<DefaultExperiment> defaultExperiment_;

    public:
        ModelDescriptionBase(const std::string &guid,
                             const std::string &fmiVersion,
                             const std::string &modelName,
                             const std::optional<std::string> &description,
                             const std::optional<std::string> &version,
                             const std::optional<std::string> &author,
                             const std::optional<std::string> &license,
                             const std::optional<std::string> &copyright,
                             const std::optional<std::string> &generationTool,
                             const std::optional<std::string> &generationDateAndTime,
                             const std::optional<std::string> &variableNamingConvention,
                             const size_t numberOfEventIndicators,
                             const ModelVariables &modelVariables,
                             const ModelStructure &modelStructure,
                             const std::optional<DefaultExperiment> defaultExperiment);

        std::string guid() const;

        std::string fmiVersion() const;

        std::string modelName() const;

        std::optional<std::string> description() const;

        std::optional<std::string> version() const;

        std::optional<std::string> author() const;

        std::optional<std::string> license() const;

        std::optional<std::string> copyright() const;

        std::optional<std::string> generationTool() const;

        std::optional<std::string> generationDateAndTime() const;

        std::optional<std::string> variableNamingConvention() const;

        size_t numberOfEventIndicators() const;

        size_t numberOfContinuousStates() const;

        ModelVariables &modelVariables();

        const ModelStructure &modelStructure() const;

        std::optional<DefaultExperiment> defaultExperiment() const;

        fmi2ValueReference getValueReference(const std::string &name) const;
        
        const ScalarVariable &getVariableByName(const std::string &name) const;

    };

    class CoSimulationModelDescription;
    class ModelExchangeModelDescription;

    class ModelDescription : public ModelDescriptionBase {

    protected:
        std::optional<CoSimulationAttributes> coSimulation_;
        std::optional<ModelExchangeAttributes> modelExchange_;

    public:

        ModelDescription(const ModelDescriptionBase &base,
                         const std::optional<CoSimulationAttributes> &coSimulation,
                         const std::optional<ModelExchangeAttributes> &modelExchange);

        bool supportsCoSimulation() const;

        bool supportsModelExchange() const;

        std::unique_ptr<CoSimulationModelDescription> asCoSimulationModelDescription() const;

        std::unique_ptr<ModelExchangeModelDescription> asModelExchangeModelDescription() const;

    };

    template <typename T>
    class SpecificModelDescription : public ModelDescriptionBase {

    protected:

        const T attributes_;

    public:

        SpecificModelDescription(const ModelDescriptionBase &base, const T &attributes)
                : ModelDescriptionBase(base), attributes_(attributes) {}

        SourceFiles sourceFiles() const {
            return attributes_.sourceFiles();
        }

        std::string modelIdentifier() const {
            return attributes_.modelIdentifier();
        }

        bool canGetAndSetFMUstate() const {
            return attributes_.canGetAndSetFMUstate();
        }

        bool canSerializeFMUstate() const {
            return attributes_.canSerializeFMUstate();
        }

        bool needsExecutionTool() const {
            return attributes_.needsExecutionTool();
        }

        bool canNotUseMemoryManagementFunctions() const {
            return attributes_.canNotUseMemoryManagementFunctions();
        }

        bool canBeInstantiatedOnlyOncePerProcess() const {
            return attributes_.canBeInstantiatedOnlyOncePerProcess();
        }

        bool providesDirectionalDerivative() const {
            return attributes_.providesDirectionalDerivative();
        }

    };

    class CoSimulationModelDescription : public SpecificModelDescription<CoSimulationAttributes> {

    public:

        CoSimulationModelDescription(const ModelDescriptionBase &base, const CoSimulationAttributes &attributes);

        bool canInterpolateInputs() const;
        bool canRunAsynchronuously() const;
        bool canHandleVariableCommunicationStepSize() const;

        size_t maxOutputDerivativeOrder() const;

    };

    class ModelExchangeModelDescription: public SpecificModelDescription<ModelExchangeAttributes> {

    public:
        ModelExchangeModelDescription(const ModelDescriptionBase &base, const ModelExchangeAttributes &attributes);

        bool completedIntegratorStepNotNeeded() const;

    };

}

#endif //FMI4CPP_MODELDESCRIPTION_HPP

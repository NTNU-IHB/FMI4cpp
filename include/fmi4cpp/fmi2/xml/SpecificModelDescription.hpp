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

#ifndef FMI4CPP_SPECIFICMODELDESCRIPTION_HPP
#define FMI4CPP_SPECIFICMODELDESCRIPTION_HPP

#include <type_traits>
#include "ModelDescription.hpp"
#include "SourceFiles.hpp"

namespace fmi4cpp::fmi2::xml {

    template <class T>
    class SpecificModelDescription : public ModelDescription {

        static_assert(std::is_base_of<xml::FmuTypeAttributes, T>::value, "T must derive from FmuTypeAttributes");

    private:
        ModelDescription &modelDescription_;

    protected:
        const T &data_;

    public:

        SpecificModelDescription(ModelDescription &md, const T &data)
                : modelDescription_(md), data_(data) {};

        string getModelIdentifier() const {
            return data_.modelIdentifier;
        }

        bool canGetAndSetFMUstate() const {
            return data_.canGetAndSetFMUstate;
        }

        bool canSerializeFMUstate() const {
            return data_.canSerializeFMUstate;
        }

        bool needsExecutionTool() const {
            return data_.needsExecutionTool;
        }

        bool canNotUseMemoryManagementFunctions() const {
            return data_.canNotUseMemoryManagementFunctions;
        }

        bool canBeInstantiatedOnlyOncePerProcess() const {
            return data_.canBeInstantiatedOnlyOncePerProcess;
        }

        bool providesDirectionalDerivative() const {
            return data_.providesDirectionalDerivative;
        }

        SourceFiles getSourceFiles() const {
            return data_.sourceFiles;
        }

        string getGuid() const override {
            return modelDescription_.getGuid();
        }

        string getFmiVersion() const override {
            return modelDescription_.getFmiVersion();
        }

        string getModelName() const override {
            return modelDescription_.getModelName();
        }

        string getDescription() const override {
            return modelDescription_.getDescription();
        }

        string getVersion() const {
            return modelDescription_.getVersion();
        }

        string getAuthor() const override {
            return modelDescription_.getAuthor();
        }

        string getLicense() const override {
            return modelDescription_.getLicense();
        }

        string getCopyright() const override {
            return modelDescription_.getCopyright();
        }

        string getGenerationTool() const override {
            return modelDescription_.getGenerationTool();
        }

        string getGenerationDateAndTime() const override {
            return modelDescription_.getGenerationDateAndTime();
        }

        string getVariableNamingConvention() const override {
            return modelDescription_.getVariableNamingConvention();
        }

        unsigned int getNumberOfEventIndicators() const override {
            return modelDescription_.getNumberOfEventIndicators();
        }

        unsigned int getNumberOfContinuousStates() const override {
            return modelDescription_.getNumberOfContinuousStates();
        }

        ModelVariables &getModelVariables() override {
            return modelDescription_.getModelVariables();
        }

        const ModelStructure &getModelStructure() const override {
            return modelDescription_.getModelStructure();
        }

        const std::optional<DefaultExperiment> getDefaultExperiment() const override {
            return modelDescription_.getDefaultExperiment();
        }

        bool supportsModelExchange() const override {
            return modelDescription_.supportsModelExchange();
        }

        bool supportsCoSimulation() const override {
            return modelDescription_.supportsCoSimulation();
        }

        ScalarVariable &getVariableByName(const string &name) override {
            return modelDescription_.getVariableByName(name);
        }

        ScalarVariable &getVariableByValueReference(const fmi2ValueReference vr) override {
            return modelDescription_.getVariableByValueReference(vr);
        }

        CoSimulationModelDescription asCoSimulationModelDescription() override;

        ModelExchangeModelDescription asModelExchangeModelDescription() override;

    };

    class CoSimulationModelDescription : public SpecificModelDescription<CoSimulationAttributes> {

    public:

        explicit CoSimulationModelDescription(ModelDescription &md, const CoSimulationAttributes &data);

        bool canInterpolateInputs() const;

        bool canRunAsynchronuously() const;

        bool canHandleVariableCommunicationStepSize() const;

        const unsigned int maxOutputDerivativeOrder() const;

    };

    class ModelExchangeModelDescription : public SpecificModelDescription<ModelExchangeAttributes> {

    public:
        explicit ModelExchangeModelDescription(ModelDescription &md, const ModelExchangeAttributes &data);

        bool completedIntegratorStepNotNeeded() const;

    };

}


#endif //FMI4CPP_SPECIFICMODELDESCRIPTION_HPP

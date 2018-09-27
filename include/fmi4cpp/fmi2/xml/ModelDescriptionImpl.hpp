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

#ifndef FMI4CPP_MODELDESCRIPTIONIMPL_HPP
#define FMI4CPP_MODELDESCRIPTIONIMPL_HPP

#include "ModelDescription.hpp"

namespace fmi4cpp::fmi2::xml {

    class ModelDescriptionImpl: public ModelDescription {

    private:
        string guid_;
        string fmiVersion_;
        string modelName_;
        string description_;
        string version_;
        string author_;
        string license_;
        string copyright_;
        string generationTool_;
        string generationDateAndTime_;
        string variableNamingConvention_;

        unsigned int numberOfEventIndicators_;

        ModelVariables modelVariables_;
        ModelStructure modelStructure_;
        std::optional<DefaultExperiment> defaultExperiment_;

        std::optional<CoSimulationAttributes> coSimulation_;
        std::optional<ModelExchangeAttributes> modelExchange_;

    public:

        explicit ModelDescriptionImpl(const string &fileName);

        string getGuid() const override;

        string getFmiVersion() const override;

        string getModelName() const override;

        string getDescription() const override;

        string getVersion() const override;

        string getAuthor() const override;

        string getLicense() const override;

        string getCopyright() const override;

        string getGenerationTool() const override;

        string getGenerationDateAndTime() const override;

        string getVariableNamingConvention() const override;

        unsigned int getNumberOfEventIndicators() const override;

        unsigned int getNumberOfContinuousStates() const override;

        ModelVariables &getModelVariables() override;

        const ModelStructure &getModelStructure() const override;

        const std::optional<DefaultExperiment> getDefaultExperiment() const override;

        bool supportsModelExchange() const override;

        bool supportsCoSimulation() const override;

        ScalarVariable &getVariableByName(const string &name) override;

        ScalarVariable &getVariableByValueReference(const fmi2ValueReference vr) override;

        CoSimulationModelDescription asCoSimulationModelDescription() override;

        ModelExchangeModelDescription asModelExchangeModelDescription() override;

    };

}


#endif //FMI4CPP_MODELDESCRIPTIONIMPL_HPP

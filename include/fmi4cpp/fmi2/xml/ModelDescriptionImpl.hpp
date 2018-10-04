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

    class ModelDescriptionImpl: public virtual ModelDescriptionProvider {

    private:
        std::string guid_;
        std::string fmiVersion_;
        std::string modelName_;
        std::string description_;
        std::string version_;
        std::string author_;
        std::string license_;
        std::string copyright_;
        std::string generationTool_;
        std::string generationDateAndTime_;
        std::string variableNamingConvention_;

        size_t numberOfEventIndicators_;

        std::unique_ptr<ModelVariables> modelVariables_;
        std::unique_ptr<ModelStructure> modelStructure_;

        std::optional<DefaultExperiment> defaultExperiment_;

        std::optional<CoSimulationAttributes> coSimulation_;
        std::optional<ModelExchangeAttributes> modelExchange_;

    public:

        explicit ModelDescriptionImpl(const std::string &fileName);

        std::string getGuid() const override;

        std::string getFmiVersion() const override;

        std::string getModelName() const override;

        std::string getDescription() const override;

        std::string getVersion() const override;

        std::string getAuthor() const override;

        std::string getLicense() const override;

        std::string getCopyright() const override;

        std::string getGenerationTool() const override;

        std::string getGenerationDateAndTime() const override;

        std::string getVariableNamingConvention() const override;

        size_t getNumberOfEventIndicators() const override;

        size_t getNumberOfContinuousStates() const override;

        const ModelVariables &getModelVariables() const override;

        const ModelStructure &getModelStructure() const override;

        std::optional<DefaultExperiment> getDefaultExperiment() const override;

        bool supportsModelExchange() const override;

        bool supportsCoSimulation() const override;

        const ScalarVariable &getVariableByName(const std::string &name) const override;

        const CoSimulationModelDescription asCoSimulationModelDescription() const override;

        const ModelExchangeModelDescription asModelExchangeModelDescription() const override;

    };

}


#endif //FMI4CPP_MODELDESCRIPTIONIMPL_HPP

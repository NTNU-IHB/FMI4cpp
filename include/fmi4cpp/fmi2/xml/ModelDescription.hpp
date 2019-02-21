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

#include "ModelStructure.hpp"
#include "ModelVariables.hpp"
#include "DefaultExperiment.hpp"
#include "FmuAttributes.hpp"

namespace fmi4cpp::fmi2 {

    struct ModelDescriptionBase {

        std::string guid;
        std::string modelName;
        std::string fmiVersion;

        std::optional<std::string> author;
        std::optional<std::string> version;
        std::optional<std::string> license;
        std::optional<std::string> copyright;
        std::optional<std::string> description;
        std::optional<std::string> generationTool;
        std::optional<std::string> generationDateAndTime;
        std::optional<std::string> variableNamingConvention;

        std::shared_ptr<const ModelVariables> modelVariables;
        std::shared_ptr<const ModelStructure> modelStructure;

        std::optional<DefaultExperiment> defaultExperiment;

        size_t numberOfEventIndicators;
        size_t numberOfContinuousStates() const;

        unsigned int getValueReference(const std::string &name) const;

        const ScalarVariable &getVariableByName(const std::string &name) const;

    };

    class CoSimulationModelDescription;
    class ModelExchangeModelDescription;

    class ModelDescription : public ModelDescriptionBase {

    protected:
        std::optional<const CoSimulationAttributes> coSimulation_;
        std::optional<const ModelExchangeAttributes> modelExchange_;

    public:

        ModelDescription(const ModelDescriptionBase &base,
                         const std::optional<const CoSimulationAttributes> &coSimulation,
                         const std::optional<const ModelExchangeAttributes> &modelExchange);

        bool supportsCoSimulation() const;

        bool supportsModelExchange() const;

        std::unique_ptr<const CoSimulationModelDescription> asCoSimulationModelDescription() const;

        std::unique_ptr<const ModelExchangeModelDescription> asModelExchangeModelDescription() const;

    };

}

#endif //FMI4CPP_MODELDESCRIPTION_HPP

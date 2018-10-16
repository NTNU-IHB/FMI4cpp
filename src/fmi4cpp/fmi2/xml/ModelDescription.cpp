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

#include <fmi4cpp/fmi2/xml/ModelDescription.hpp>

using namespace fmi4cpp::fmi2::xml;

ModelDescriptionBase::ModelDescriptionBase(const std::string &guid,
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
                                           const std::shared_ptr<ModelVariables> &modelVariables,
                                           const std::shared_ptr<ModelStructure> &modelStructure,
                                           const std::optional<DefaultExperiment> &defaultExperiment)
        : guid_(guid), fmiVersion_(fmiVersion), modelName_(modelName), description_(description),
        version_(version), author_(author), license_(license), copyright_(copyright), generationTool_(generationTool),
        generationDateAndTime_(generationDateAndTime), variableNamingConvention_(variableNamingConvention),
        numberOfEventIndicators_(numberOfEventIndicators), modelVariables_(modelVariables),
        modelStructure_(modelStructure), defaultExperiment_(defaultExperiment) {}

std::string ModelDescriptionBase::guid() const {
    return guid_;
}

std::string ModelDescriptionBase::fmiVersion() const {
    return fmiVersion_;
}

std::string ModelDescriptionBase::modelName() const {
    return modelName_;
}

std::optional<std::string> ModelDescriptionBase::description() const {
    return description_;
}

std::optional<std::string> ModelDescriptionBase::version() const {
    return version_;
}

std::optional<std::string> ModelDescriptionBase::author() const {
    return author_;
}

std::optional<std::string> ModelDescriptionBase::license() const {
    return license_;
}

std::optional<std::string> ModelDescriptionBase::copyright() const {
    return copyright_;
}

std::optional<std::string> ModelDescriptionBase::generationTool() const {
    return generationTool_;
}

std::optional<std::string> ModelDescriptionBase::generationDateAndTime() const {
    return generationDateAndTime_;
}

std::optional<std::string> ModelDescriptionBase::variableNamingConvention() const {
    return variableNamingConvention_;
}

size_t ModelDescriptionBase::numberOfEventIndicators() const {
    return numberOfEventIndicators_;
}

size_t ModelDescriptionBase::numberOfContinuousStates() const {
    return modelStructure_->derivatives().size();
}
 
const std::shared_ptr<ModelVariables> &ModelDescriptionBase::modelVariables() const {
    return modelVariables_;
}

const std::shared_ptr<ModelStructure> &ModelDescriptionBase::modelStructure() const {
    return modelStructure_;
}

std::optional<DefaultExperiment> ModelDescriptionBase::defaultExperiment() const {
    return defaultExperiment_;
}

const ScalarVariable &ModelDescriptionBase::getVariableByName(const std::string &name) const {
    return modelVariables_->getByName(name);
}

fmi2ValueReference ModelDescriptionBase::getValueReference(const std::string &name) const {
    return modelVariables_->getByName(name).valueReference();
}

ModelDescription::ModelDescription(const ModelDescriptionBase &base,
                                   const std::optional<CoSimulationAttributes> &coSimulation,
                                   const std::optional<ModelExchangeAttributes> &modelExchange)
        : ModelDescriptionBase(base), coSimulation_(coSimulation), modelExchange_(modelExchange) {}

bool ModelDescription::supportsCoSimulation() const {
    return coSimulation_.has_value();
}

bool ModelDescription::supportsModelExchange() const {
    return modelExchange_.has_value();
}

std::unique_ptr<CoSimulationModelDescription> ModelDescription::asCoSimulationModelDescription() const {
    if (!supportsCoSimulation()) {
        throw std::runtime_error("CoSimulation not supported!");
    }
    return std::make_unique<CoSimulationModelDescription>(*this, *coSimulation_);
}

std::unique_ptr<ModelExchangeModelDescription> ModelDescription::asModelExchangeModelDescription() const {
    if (!supportsModelExchange()) {
        throw std::runtime_error("ModelExchange not supported!");
    }
    return std::make_unique<ModelExchangeModelDescription>(*this, *modelExchange_);
}

CoSimulationModelDescription::CoSimulationModelDescription(const ModelDescriptionBase &base,
                                                           const CoSimulationAttributes &attributes)
        : SpecificModelDescription(base, attributes) {}

bool CoSimulationModelDescription::canInterpolateInputs() const {
    return attributes_.canInterpolateInputs;
}

bool CoSimulationModelDescription::canRunAsynchronuously() const {
    return attributes_.canRunAsynchronuously;
}

bool CoSimulationModelDescription::canHandleVariableCommunicationStepSize() const {
    return attributes_.canHandleVariableCommunicationStepSize;
}

size_t CoSimulationModelDescription::maxOutputDerivativeOrder() const {
    return attributes_.maxOutputDerivativeOrder;
}

ModelExchangeModelDescription::ModelExchangeModelDescription(const ModelDescriptionBase &base,
                                                             const ModelExchangeAttributes &attributes)
        : SpecificModelDescription(base, attributes) {}

bool ModelExchangeModelDescription::completedIntegratorStepNotNeeded() const {
    return attributes_.completedIntegratorStepNotNeeded;
}

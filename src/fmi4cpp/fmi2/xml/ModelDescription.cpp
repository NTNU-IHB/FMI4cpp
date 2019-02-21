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

#include <fmi4cpp/fmi2/xml/CoSimulationModelDescription.hpp>
#include <fmi4cpp/fmi2/xml/ModelExchangeModelDescription.hpp>

using namespace fmi4cpp::fmi2;

size_t ModelDescriptionBase::numberOfContinuousStates() const {
    return modelStructure->derivatives.size();
}

const ScalarVariable &ModelDescriptionBase::getVariableByName(const std::string &name) const {
    return modelVariables->getByName(name);
}

fmi2ValueReference ModelDescriptionBase::getValueReference(const std::string &name) const {
    return modelVariables->getByName(name).valueReference;
}

ModelDescription::ModelDescription(const ModelDescriptionBase &base,
                                   const std::optional<const CoSimulationAttributes> &coSimulation,
                                   const std::optional<const ModelExchangeAttributes> &modelExchange)
        : ModelDescriptionBase(base), coSimulation_(coSimulation), modelExchange_(modelExchange) {}

bool ModelDescription::supportsCoSimulation() const {
    return coSimulation_.has_value();
}

bool ModelDescription::supportsModelExchange() const {
    return modelExchange_.has_value();
}

std::unique_ptr<const CoSimulationModelDescription> ModelDescription::asCoSimulationModelDescription() const {
    if (!supportsCoSimulation()) {
        throw std::runtime_error("CoSimulation not supported!");
    }
    return std::make_unique<CoSimulationModelDescription>(*this, *coSimulation_);
}

std::unique_ptr<const ModelExchangeModelDescription> ModelDescription::asModelExchangeModelDescription() const {
    if (!supportsModelExchange()) {
        throw std::runtime_error("ModelExchange not supported!");
    }
    return std::make_unique<ModelExchangeModelDescription>(*this, *modelExchange_);
}

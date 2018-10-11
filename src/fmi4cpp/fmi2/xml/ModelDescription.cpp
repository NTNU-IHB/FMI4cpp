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

#include <fmi4cpp/fmi2/xml/ModelDescriptionImpl.hpp>
#include <fmi4cpp/fmi2/xml/SpecificModelDescription.hpp>
#include <fmi4cpp/fmi2/xml/ModelDescription.hpp>


using namespace std;
using namespace fmi4cpp::fmi2::xml;

namespace {

    const string DEFAULT_VARIABLE_NAMING_CONVENTION = "flat";

}

ModelDescription::ModelDescription(const string &guid,
                                   const string &fmiVersion, 
                                   const string &modelName,
                                   const optional<string> &description,
                                   const optional<string> &version,
                                   const optional<string> &author,
                                   const optional<string> &license,
                                   const optional<string> &copyright,
                                   const optional<string> &generationTool,
                                   const optional<string> &generationDateAndTime,
                                   const optional<string> &variableNamingConvention,
                                   const size_t numberOfEventIndicators,
                                   const ModelVariables &modelVariables, 
                                   const ModelStructure &modelStructure,
                                   const optional<DefaultExperiment> &defaultExperiment,
                                   const optional<CoSimulationAttributes> &coSimulation,
                                   const optional<ModelExchangeAttributes> &modelExchange)
        : guid_(guid),
          fmiVersion_(fmiVersion),
          modelName_(modelName),
          description_(description),
          version_(version),
          author_(author),
          license_(license),
          copyright_(copyright),
          generationTool_(generationTool),
          generationDateAndTime_(generationDateAndTime),
          variableNamingConvention_(variableNamingConvention),
          numberOfEventIndicators_(numberOfEventIndicators),
          modelVariables_(modelVariables),
          modelStructure_(modelStructure),
          defaultExperiment_(defaultExperiment),
          coSimulation_(coSimulation),
          modelExchange_(modelExchange) {}

std::string ModelDescription::guid() const {
    return guid_;
}

std::string ModelDescription::fmiVersion() const {
    return fmiVersion_;
}

std::string ModelDescription::modelName() const {
    return modelName_;
}

std::optional<std::string> ModelDescription::description() const {
    return description_;
}

std::optional<std::string> ModelDescription::version() const {
    return version_;
}

std::optional<std::string> ModelDescription::author() const {
    return author_;
}

std::optional<std::string> ModelDescription::license() const {
    return license_;
}

std::optional<std::string> ModelDescription::copyright() const {
    return copyright_;
}

std::optional<std::string> ModelDescription::generationTool() const {
    return generationTool_;
}

std::optional<std::string> ModelDescription::generationDateAndTime() const {
    return generationDateAndTime_;
}

std::optional<std::string> ModelDescription::variableNamingConvention() const {
    return variableNamingConvention_
}

size_t ModelDescription::numberOfEventIndicators() const {
    return numberOfEventIndicators_;
}

size_t ModelDescription::numberOfContinuousStates() const {
    return modelStructure().getDerivatives().size();
}

ModelVariables ModelDescription::modelVariables() const {
    return modelVariables_;
}

ModelStructure ModelDescription::modelStructure() const {
    return modelStructure_;
}

std::optional<DefaultExperiment> ModelDescription::defaultExperiment() const {
    return defaultExperiment_;
}

std::optional<CoSimulationAttributes> ModelDescription::coSimulation() const {
    return coSimulation_;
}

std::optional<ModelExchangeAttributes> ModelDescription::modelExchange() const {
    return modelExchange_;
}

const ScalarVariable &ModelDescription::getVariableByName(const std::string &name) const {
    return modelVariables_.getByName(name);
}

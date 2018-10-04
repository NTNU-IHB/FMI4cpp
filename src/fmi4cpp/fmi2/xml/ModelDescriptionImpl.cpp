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

using namespace std;
using namespace fmi4cpp::fmi2::xml;

namespace {

    const string DEFAULT_VARIABLE_NAMING_CONVENTION = "flat";

}

ModelDescriptionImpl::ModelDescriptionImpl(const string &fileName) {

    ptree tree;
    read_xml(fileName, tree);

    ptree root = tree.get_child("fmiModelDescription");

    guid_ = root.get<string>("<xmlattr>.guid");
    fmiVersion_ = root.get<string>("<xmlattr>.fmiVersion");
    modelName_ = root.get<string>("<xmlattr>.modelName");
    description_ = root.get<string>("<xmlattr>.description", "");
    author_ = root.get<string>("<xmlattr>.author", "");
    version_ = root.get<string>("<xmlattr>.version", "");
    license_ = root.get<string>("<xmlattr>.license", "");
    generationTool_ = root.get<string>("<xmlattr>.generationTool", "");
    generationDateAndTime_ = root.get<string>("<xmlattr>.generationDateAndTime", "");
    numberOfEventIndicators_ = root.get<unsigned int>("<xmlattr>.numberOfEventIndicators", 0);
    variableNamingConvention_ = root.get<string>("<xmlattr>.variableNamingConvention", DEFAULT_VARIABLE_NAMING_CONVENTION);

    for (const ptree::value_type &v : root) {

        if (v.first == "CoSimulation") {
            coSimulation_ = CoSimulationAttributes();
            coSimulation_->load(v.second);
        } else if (v.first == "ModelExchange") {
            modelExchange_ = ModelExchangeAttributes();
            modelExchange_->load(v.second);
        } else if (v.first == "DefaultExperiment") {
            defaultExperiment_ = DefaultExperiment();
            defaultExperiment_->load(v.second);
        } else if (v.first == "ModelVariables") {
            modelVariables_ = make_shared<ModelVariablesImpl>(v.second);
        } else if (v.first == "ModelStructure") {
            modelStructure_ = make_shared<ModelStructureImpl>(v.second);
        }

    }

}

string ModelDescriptionImpl::getGuid() const {
    return guid_;
}

string ModelDescriptionImpl::getFmiVersion() const {
    return fmiVersion_;
}

string ModelDescriptionImpl::getModelName() const {
    return modelName_;
}

string ModelDescriptionImpl::getDescription() const {
    return description_;
}

string ModelDescriptionImpl::getVersion() const {
    return version_;
}

string ModelDescriptionImpl::getAuthor() const {
    return author_;
}

string ModelDescriptionImpl::getLicense() const {
    return license_;
}

string ModelDescriptionImpl::getCopyright() const {
    return copyright_;
}

string ModelDescriptionImpl::getGenerationTool() const {
    return generationTool_;
}

string ModelDescriptionImpl::getGenerationDateAndTime() const {
    return generationDateAndTime_;
}

string ModelDescriptionImpl::getVariableNamingConvention() const {
    return variableNamingConvention_;
}

size_t ModelDescriptionImpl::getNumberOfEventIndicators() const {
    return numberOfEventIndicators_;
}

size_t ModelDescriptionImpl::getNumberOfContinuousStates() const {
    return modelStructure_->getDerivatives().size();
}
 ModelVariables &ModelDescriptionImpl::getModelVariables() const {
    return *modelVariables_;
}
 ModelStructure &ModelDescriptionImpl::getModelStructure() const {
    return *modelStructure_;
}

std::optional<DefaultExperiment> ModelDescriptionImpl::getDefaultExperiment() const {
    return defaultExperiment_;
}

bool ModelDescriptionImpl::supportsModelExchange() const {
    return modelExchange_.has_value();
}

bool ModelDescriptionImpl::supportsCoSimulation() const {
    return coSimulation_.has_value();
}

const ScalarVariable &ModelDescriptionImpl::getVariableByName(const string &name) const{
    return modelVariables_->getByName(name);
}

const CoSimulationModelDescription ModelDescriptionImpl::asCoSimulationModelDescription() const {
    return CoSimulationModelDescription(*this, *coSimulation_);
}

const ModelExchangeModelDescription ModelDescriptionImpl::asModelExchangeModelDescription() const {
    return ModelExchangeModelDescription(*this, *modelExchange_);
}

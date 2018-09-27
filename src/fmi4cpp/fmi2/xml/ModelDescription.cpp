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
#include <fmi4cpp/fmi2/xml/SpecificModelDescription.hpp>

using namespace std;
using namespace fmi4cpp::fmi2::xml;

void ModelDescription::load(const string &fileName) {

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
    variableNamingConvention_ = root.get<string>("<xmlattr>.variableNamingConvention", "flat");
    numberOfEventIndicators_ = root.get<unsigned int>("<xmlattr>.numberOfEventIndicators", 0);

    for (const ptree::value_type &v : root) {

        if (v.first == "CoSimulation") {
            coSimulation_ = make_shared<CoSimulationAttributes>();
            coSimulation_->load(v.second);
        } else if (v.first == "ModelExchange") {
            modelExchange_ = make_shared<ModelExchangeAttributes>();
            modelExchange_->load(v.second);
        } else if (v.first == "DefaultExperiment") {
            defaultExperiment_ = DefaultExperiment();
            defaultExperiment_->load(v.second);
        } else if (v.first == "ModelVariables") {
            modelVariables_.load(v.second);
        } else if (v.first == "ModelStructure") {
            modelStructure_.load(v.second);
        }

    }

}

string ModelDescription::getGuid() const {
    return guid_;
}

string ModelDescription::getFmiVersion() const {
    return fmiVersion_;
}

string ModelDescription::getModelName() const {
    return modelName_;
}

string ModelDescription::getDescription() const {
    return description_;
}

string ModelDescription::getVersion() const {
    return version_;
}

string ModelDescription::getAuthor() const {
    return author_;
}

string ModelDescription::getLicense() const {
    return license_;
}

string ModelDescription::getCopyright() const {
    return copyright_;
}

string ModelDescription::getGenerationTool() const {
    return generationTool_;
}

string ModelDescription::getGenerationDateAndTime() const {
    return generationDateAndTime_;
}

string ModelDescription::getVariableNamingConvention() const {
    return variableNamingConvention_;
}

unsigned int ModelDescription::getNumberOfEventIndicators() const {
    return numberOfEventIndicators_;
}

unsigned int ModelDescription::getnNumberOfContinuousStates() const {
    return modelStructure_.getDerivatives().size();
}

const ModelVariables &ModelDescription::getModelVariables() const {
    return modelVariables_;
}

const ModelStructure &ModelDescription::getModelStructure() const {
    return modelStructure_;
}

const std::optional<DefaultExperiment> ModelDescription::defaultExperiment() const {
    return defaultExperiment_;
}

bool ModelDescription::supportsModelExchange() const {
    return modelExchange_ != nullptr;
}

bool ModelDescription::supportsCoSimulation() const {
    return coSimulation_ != nullptr;
}

shared_ptr<CoSimulationModelDescription> ModelDescription::asCoSimulationModelDescription() const {
    return make_shared<CoSimulationModelDescription>(*this, *coSimulation_);
}

shared_ptr<ModelExchangeModelDescription> ModelDescription::asModelExchangeModelDescription() const {
    return make_shared<ModelExchangeModelDescription>(*this, *modelExchange_);
}

ScalarVariable ModelDescription::getVariableByName(const string &name) {
    return modelVariables_.getByName(name);
}

ScalarVariable ModelDescription::getVariableByValueReference(const fmi2ValueReference vr) {
    return modelVariables_.getByValueReference(vr);
}

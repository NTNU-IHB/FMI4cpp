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

#include <fmicpp/fmi2/xml/ModelDescription.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;

void ModelDescription::load(const string fileName) {

    ptree tree;
    read_xml(fileName, tree);

    ptree root = tree.get_child("fmiModelDescription");

    guid = root.get<string>("<xmlattr>.guid");
    fmiVersion = root.get<string>("<xmlattr>.fmiVersion");
    modelName = root.get<string>("<xmlattr>.modelName");
    description = root.get<string>("<xmlattr>.description", "");
    author = root.get<string>("<xmlattr>.author", "");
    version = root.get<string>("<xmlattr>.version", "");
    license = root.get<string>("<xmlattr>.license", "");
    generationTool = root.get<string>("<xmlattr>.generationTool", "");
    generationDateAndTime = root.get<string>("<xmlattr>.generationDateAndTime", "");

    numberOfEventIndicators = root.get<unsigned int>("<xmlattr>.numberOfEventIndicators", 0);

    for (const ptree::value_type &v : root) {

        if (v.first == "CoSimulation") {
            coSimulation = make_shared<CoSimulationAttributes>(CoSimulationAttributes{});
            coSimulation->load(v.second);
            supportsCoSimulation = true;
        } else if (v.first == "ModelExchange") {
            modelExchange = make_shared<ModelExchangeAttributes>(ModelExchangeAttributes{});
            modelExchange->load(v.second);
            supportsModelExchange = true;
        } else if (v.first == "DefaultExperiment") {
            defaultExperiment = make_shared<DefaultExperiment>(DefaultExperiment());
            defaultExperiment->load(v.second);
        } else if (v.first == "ModelVariables") {
            modelVariables = make_shared<ModelVariables>(ModelVariables());
            modelVariables->load(v.second);
        }

    }

}

shared_ptr<CoSimulationModelDescription> ModelDescription::asCoSimulationFmu() const {
    return make_shared<CoSimulationModelDescription>(*this, *coSimulation);
}

shared_ptr<ModelExchangeModelDescription> ModelDescription::asModelExchangeFmu() const {
    return make_shared<ModelExchangeModelDescription>(*this, *modelExchange);
}

ScalarVariable &ModelDescription::getVariableByName(const string &name) const {
    return modelVariables->getByName(name);
}

ScalarVariable &ModelDescription::getVariableByValueReference(const fmi2ValueReference vr) const {
    return modelVariables->getByValueReference(vr);
}

SpecificModelDescription::SpecificModelDescription(const ModelDescription md, const FmuTypeAttributes data)
        : ModelDescription(md),
        modelIdentifier(data.modelIdentifier),
        canGetAndSetFMUstate(data.canGetAndSetFMUstate),
        needsExecutionTool(data.needsExecutionTool),
        canNotUseMemoryManagementFunctions(data.canNotUseMemoryManagementFunctions),
        canBeInstantiatedOnlyOncePerProcess(data.canBeInstantiatedOnlyOncePerProcess),
        providesDirectionalDerivative(data.providesDirectionalDerivative),
        sourceFiles(data.sourceFiles),
        canSerializeFMUstate(data.canSerializeFMUstate) {};

CoSimulationModelDescription::CoSimulationModelDescription(const ModelDescription md, const CoSimulationAttributes data)
        : SpecificModelDescription(md, data),
        canInterpolateInputs(data.canInterpolateInputs),
        canRunAsynchronuously(data.canRunAsynchronuously),
        canHandleVariableCommunicationStepSize(data.canHandleVariableCommunicationStepSize),
        maxOutputDerivativeOrder(data.maxOutputDerivativeOrder) {};

ModelExchangeModelDescription::ModelExchangeModelDescription(const ModelDescription md, const ModelExchangeAttributes data)
        : SpecificModelDescription(md, data),
        numberOfEventIndicators(data.numberOfEventIndicators),
        completedIntegratorStepNotNeeded(data.completedIntegratorStepNotNeeded) {};

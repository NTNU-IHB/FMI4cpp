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

void FmuData::load(const ptree &node) {

    modelIdentifier = node.get<string>("<xmlattr>.modelIdentifier");

    needsExecutionTool = node.get<bool>("xmlattr>.needsExecutionTool", false);
    canGetAndSetFMUstate = node.get<bool>("xmlattr>.canGetAndSetFMUstate", false);
    canSerializeFMUstate = node.get<bool>("xmlattr>.canSerializeFMUstate", false);
    providesDirectionalDerivative = node.get<bool>("xmlattr>.providesDirectionalDerivative", false);
    canNotUseMemoryManagementFunctions = node.get<bool>("xmlattr>.canNotUseMemoryManagementFunctions", false);
    canBeInstantiatedOnlyOncePerProcess = node.get<bool>("xmlattr>.canBeInstantiatedOnlyOncePerProcess", false);

    for (const ptree::value_type &v : node) {
        if (v.first == "SourceFiles") {
            sourceFiles = make_shared<SourceFiles>(SourceFiles());
            sourceFiles->load(v.second);
        }
    }

}

void CoSimulationData::load(const ptree &node) {
    FmuData::load(node);

    maxOutputDerivativeOrder = node.get<unsigned int>("<xmlattr>.maxOutputDerivativeOrder", 0);

    canInterpolateInputs = node.get<bool>("<xmlattr>.canInterpolateInputs", false);
    canRunAsynchronuously = node.get<bool>("<xmlattr>.canRunAsynchronuously", false);
    canHandleVariableCommunicationStepSize = node.get<bool>("<xmlattr>.canHandleVariableCommunicationStepSize", false);

}

void ModelExchangeData::load(const ptree &node) {
    FmuData::load(node);

    numberOfEventIndicators = node.get<unsigned int>("<xmlattr>.numberOfEventIndicators", 0);
    completedIntegratorStepNotNeeded = node.get<bool>("<xmlattr>.completedIntegratorStepNotNeeded", false);

}

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

    numberOfEventIndicators = root.get<int>("<xmlattr>.numberOfEventIndicators", 0);

    for (const ptree::value_type &v : root) {

        if (v.first == "CoSimulation") {
            coSimulation = make_unique<CoSimulationData>(CoSimulationData{});
            coSimulation->load(v.second);
        } else if (v.first == "ModelExchange") {
            modelExchange = make_unique<ModelExchangeData>(ModelExchangeData{});
            modelExchange->load(v.second);
        } else if (v.first == "DefaultExperiment") {
            defaultExperiment = make_unique<DefaultExperiment>(DefaultExperiment());
            defaultExperiment->load(v.second);
        } else if (v.first == "ModelVariables") {
            modelVariables = make_unique<ModelVariables>(ModelVariables());
            modelVariables->load(v.second);
        }

    }

}

SpecificModelDescription::SpecificModelDescription(const FmuData &data)
        : modelIdentifier(data.modelIdentifier),
        canGetAndSetFMUstate(data.canGetAndSetFMUstate),
        needsExecutionTool(data.needsExecutionTool),
        canNotUseMemoryManagementFunctions(data.canNotUseMemoryManagementFunctions),
        canBeInstantiatedOnlyOncePerProcess(data.canBeInstantiatedOnlyOncePerProcess),
        providesDirectionalDerivative(data.providesDirectionalDerivative),
        sourceFiles(data.sourceFiles),
        canSerializeFMUstate(data.canSerializeFMUstate) {};


CoSimulationModelDescription::CoSimulationModelDescription(const CoSimulationData data)
        : SpecificModelDescription(data),
        canInterpolateInputs(data.canInterpolateInputs),
        canRunAsynchronuously(data.canRunAsynchronuously),
        canHandleVariableCommunicationStepSize(data.canHandleVariableCommunicationStepSize),
        maxOutputDerivativeOrder(data.maxOutputDerivativeOrder) {};

ModelExchangeModelDescription::ModelExchangeModelDescription(const ModelExchangeData data)
        : SpecificModelDescription(data),
        numberOfEventIndicators(data.numberOfEventIndicators),
        completedIntegratorStepNotNeeded(data.completedIntegratorStepNotNeeded) {};

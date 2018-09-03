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

#include <fmicpp/fmi2/xml/FmuData.hpp>

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

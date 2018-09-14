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
#include <fmicpp/fmi2/xml/SpecificModelDescription.hpp>

using namespace fmicpp::fmi2::xml;

SpecificModelDescription::SpecificModelDescription(const ModelDescription &md, const FmuTypeAttributes &data)
        : ModelDescription(md),
          modelIdentifier_(data.modelIdentifier),
          canGetAndSetFMUstate_(data.canGetAndSetFMUstate),
          needsExecutionTool_(data.needsExecutionTool),
          canNotUseMemoryManagementFunctions_(data.canNotUseMemoryManagementFunctions),
          canBeInstantiatedOnlyOncePerProcess_(data.canBeInstantiatedOnlyOncePerProcess),
          providesDirectionalDerivative_(data.providesDirectionalDerivative),
          sourceFiles_(data.sourceFiles),
          canSerializeFMUstate_(data.canSerializeFMUstate) {};

string SpecificModelDescription::modelIdentifier() const {
    return modelIdentifier_;
}

bool SpecificModelDescription::canGetAndSetFMUstate() const {
    return canGetAndSetFMUstate_;
}

bool SpecificModelDescription::canSerializeFMUstate() const {
    return canSerializeFMUstate_;
}

bool SpecificModelDescription::needsExecutionTool() const {
    return needsExecutionTool_;
}

bool SpecificModelDescription::canNotUseMemoryManagementFunctions() const {
    return canNotUseMemoryManagementFunctions_;
}

bool SpecificModelDescription::canBeInstantiatedOnlyOncePerProcess() const {
    return canBeInstantiatedOnlyOncePerProcess_;
}

bool SpecificModelDescription::providesDirectionalDerivative() const {
    return providesDirectionalDerivative_;
}

SourceFiles SpecificModelDescription::sourceFiles() const {
    return sourceFiles_;
}

CoSimulationModelDescription::CoSimulationModelDescription(const ModelDescription &md,
                                                           const CoSimulationAttributes &data)
        : SpecificModelDescription(md, data),
          canInterpolateInputs_(data.canInterpolateInputs),
          canRunAsynchronuously_(data.canRunAsynchronuously),
          canHandleVariableCommunicationStepSize_(data.canHandleVariableCommunicationStepSize),
          maxOutputDerivativeOrder_(data.maxOutputDerivativeOrder) {};

bool CoSimulationModelDescription::canInterpolateInputs() const {
    return canInterpolateInputs_;
}

bool CoSimulationModelDescription::canRunAsynchronuously() const {
    return canRunAsynchronuously_;
}

bool CoSimulationModelDescription::canHandleVariableCommunicationStepSize() const {
    return canHandleVariableCommunicationStepSize_;
}

const unsigned int CoSimulationModelDescription::maxOutputDerivativeOrder() const {
    return maxOutputDerivativeOrder_;
}

ModelExchangeModelDescription::ModelExchangeModelDescription(const ModelDescription &md,
                                                             const ModelExchangeAttributes &data)
        : SpecificModelDescription(md, data),
          completedIntegratorStepNotNeeded_(data.completedIntegratorStepNotNeeded) {};

bool ModelExchangeModelDescription::completedIntegratorStepNotNeeded() const {
    return completedIntegratorStepNotNeeded_;
}

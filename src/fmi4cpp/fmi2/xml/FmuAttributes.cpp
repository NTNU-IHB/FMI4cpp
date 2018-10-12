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

#include <fmi4cpp/fmi2/xml/FmuAttributes.hpp>
#include <optional>

using namespace fmi4cpp::fmi2::xml;

FmuAttributes::FmuAttributes(const std::string &modelIdentifier, const bool canGetAndSetFMUstate,
                             const bool canSerializeFMUstate, const bool needsExecutionTool,
                             const bool canNotUseMemoryManagementFunctions,
                             const bool canBeInstantiatedOnlyOncePerProcess,
                             const bool providesDirectionalDerivative,
                             const SourceFiles &sourceFiles)
        : modelIdentifier_(modelIdentifier),
          canGetAndSetFMUstate_(canGetAndSetFMUstate),
          canSerializeFMUstate_(canSerializeFMUstate),
          needsExecutionTool_(needsExecutionTool),
          canNotUseMemoryManagementFunctions_(canNotUseMemoryManagementFunctions),
          canBeInstantiatedOnlyOncePerProcess_(canBeInstantiatedOnlyOncePerProcess),
          providesDirectionalDerivative_(providesDirectionalDerivative),
          sourceFiles_(sourceFiles) {}

bool FmuAttributes::canGetAndSetFMUstate() const {
    return canGetAndSetFMUstate_;
}

bool FmuAttributes::canSerializeFMUstate() const {
    return canSerializeFMUstate_;
}

bool FmuAttributes::needsExecutionTool() const {
    return needsExecutionTool_;
}

bool FmuAttributes::canNotUseMemoryManagementFunctions() const {
    return canNotUseMemoryManagementFunctions_;
}

bool FmuAttributes::canBeInstantiatedOnlyOncePerProcess() const {
    return canBeInstantiatedOnlyOncePerProcess_;
}

bool FmuAttributes::providesDirectionalDerivative() const {
    return providesDirectionalDerivative_;
}

SourceFiles FmuAttributes::sourceFiles() const {
    return sourceFiles_;
}

std::string FmuAttributes::modelIdentifier() const {
    return modelIdentifier_;
}

CoSimulationAttributes::CoSimulationAttributes(const FmuAttributes &attributes,
                                               const bool canInterpolateInputs,
                                               const bool canRunAsynchronuously,
                                               const bool canHandleVariableCommunicationStepSize,
                                               const size_t maxOutputDerivativeOrder)
        : FmuAttributes(attributes),
          canInterpolateInputs_(canInterpolateInputs),
          canRunAsynchronuously_(canRunAsynchronuously),
          canHandleVariableCommunicationStepSize_(canHandleVariableCommunicationStepSize),
          maxOutputDerivativeOrder_(maxOutputDerivativeOrder) {}

bool CoSimulationAttributes::canInterpolateInputs() const {
    return canInterpolateInputs_;
}

bool CoSimulationAttributes::canRunAsynchronuously() const {
    return canRunAsynchronuously_;
}

bool CoSimulationAttributes::canHandleVariableCommunicationStepSize() const {
    return canHandleVariableCommunicationStepSize_;
}

size_t CoSimulationAttributes::maxOutputDerivativeOrder() const {
    return maxOutputDerivativeOrder_;
}

ModelExchangeAttributes::ModelExchangeAttributes(const FmuAttributes &attributes,
                                                 const bool completedIntegratorStepNotNeeded)
        : FmuAttributes(attributes), completedIntegratorStepNotNeeded_(completedIntegratorStepNotNeeded) {}

bool ModelExchangeAttributes::completedIntegratorStepNotNeeded() const {
    return completedIntegratorStepNotNeeded_;
}

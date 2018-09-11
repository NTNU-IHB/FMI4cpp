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
          completedIntegratorStepNotNeeded(data.completedIntegratorStepNotNeeded) {};
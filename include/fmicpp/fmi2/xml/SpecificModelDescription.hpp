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

#ifndef FMICPP_SPECIFICMODELDESCRIPTION_HPP
#define FMICPP_SPECIFICMODELDESCRIPTION_HPP

#include "ModelDescription.hpp"
#include "SourceFiles.hpp"

namespace fmicpp::fmi2::xml {

    class SpecificModelDescription : public ModelDescription {

    private:
        const string modelIdentifier_;

        const bool canGetAndSetFMUstate_;
        const bool canSerializeFMUstate_;
        const bool needsExecutionTool_;
        const bool canNotUseMemoryManagementFunctions_;
        const bool canBeInstantiatedOnlyOncePerProcess_;
        const bool providesDirectionalDerivative_;

        const SourceFiles sourceFiles_;

    public:

        explicit SpecificModelDescription(
                const ModelDescription &modelDescription,
                const FmuTypeAttributes &data);

        string modelIdentifier() const;

        bool canGetAndSetFMUstate() const;

        bool canSerializeFMUstate() const;

        bool needsExecutionTool() const;

        bool canNotUseMemoryManagementFunctions() const;

        bool canBeInstantiatedOnlyOncePerProcess() const;

        bool providesDirectionalDerivative() const;

        SourceFiles sourceFiles() const;

    };

    class CoSimulationModelDescription : public SpecificModelDescription {

    private:
        const bool canInterpolateInputs_;
        const bool canRunAsynchronuously_;
        const bool canHandleVariableCommunicationStepSize_;

        const unsigned int maxOutputDerivativeOrder_;

    public:

        explicit CoSimulationModelDescription(
                const ModelDescription &modelDescription,
                const CoSimulationAttributes &data);

        bool canInterpolateInputs() const;

        bool canRunAsynchronuously() const;

        bool canHandleVariableCommunicationStepSize() const;

        const unsigned int maxOutputDerivativeOrder() const;

    };

    class ModelExchangeModelDescription : public SpecificModelDescription {

    private:
        const bool completedIntegratorStepNotNeeded_;

    public:
        explicit ModelExchangeModelDescription(
                const ModelDescription &modelDescription,
                const ModelExchangeAttributes &data);

        bool completedIntegratorStepNotNeeded() const;

    };

}


#endif //FMICPP_SPECIFICMODELDESCRIPTION_HPP

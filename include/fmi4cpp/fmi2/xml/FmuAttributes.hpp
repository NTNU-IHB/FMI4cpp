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

#ifndef FMI4CPP_FMUTYPESATTRIBUTE_HPP
#define FMI4CPP_FMUTYPESATTRIBUTE_HPP

#include <string>

#include "SourceFiles.hpp"

namespace fmi4cpp::fmi2::xml {

    class FmuAttributes {

    private:

        bool canGetAndSetFMUstate_;
        bool canSerializeFMUstate_;
        bool needsExecutionTool_;
        bool canNotUseMemoryManagementFunctions_;
        bool canBeInstantiatedOnlyOncePerProcess_;
        bool providesDirectionalDerivative_;

        SourceFiles sourceFiles_;
        std::string modelIdentifier_;

    public:

        FmuAttributes(const std::string &modelIdentifier, const bool canGetAndSetFMUstate,
                      const bool canSerializeFMUstate, const bool needsExecutionTool,
                      const bool canNotUseMemoryManagementFunctions, const bool canBeInstantiatedOnlyOncePerProcess,
                      const bool providesDirectionalDerivative, const SourceFiles &sourceFiles);

        SourceFiles sourceFiles() const;
        std::string modelIdentifier() const;

        bool canGetAndSetFMUstate() const;
        bool canSerializeFMUstate() const;
        bool needsExecutionTool() const;
        bool canNotUseMemoryManagementFunctions() const;
        bool canBeInstantiatedOnlyOncePerProcess() const;
        bool providesDirectionalDerivative() const;

    };

    class CoSimulationAttributes : public FmuAttributes {

    private:
        bool canInterpolateInputs_;
        bool canRunAsynchronuously_;
        bool canHandleVariableCommunicationStepSize_;

        size_t maxOutputDerivativeOrder_;

    public:

        CoSimulationAttributes(const FmuAttributes &attributes, const bool canInterpolateInputs,
                               const bool canRunAsynchronuously, const bool canHandleVariableCommunicationStepSize,
                               const size_t maxOutputDerivativeOrder);

        bool canInterpolateInputs() const;
        bool canRunAsynchronuously() const;
        bool canHandleVariableCommunicationStepSize() const;

        size_t maxOutputDerivativeOrder() const;

    };

    class ModelExchangeAttributes : public FmuAttributes {

    private:
        bool completedIntegratorStepNotNeeded_;

    public:
        ModelExchangeAttributes(const FmuAttributes &attributes,
                                const bool completedIntegratorStepNotNeeded);

        bool completedIntegratorStepNotNeeded() const;

    };

}

#endif //FMI4CPP_FMUTYPESATTRIBUTE_HPP

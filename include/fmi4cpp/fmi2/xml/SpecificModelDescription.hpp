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

#ifndef FMI4CPP_SPECIFICMODELDESCRIPTION_HPP
#define FMI4CPP_SPECIFICMODELDESCRIPTION_HPP

#include "ModelDescription.hpp"

namespace fmi4cpp::fmi2 {

    template <typename T>
    class SpecificModelDescription : public ModelDescriptionBase {

    protected:

        const T attributes_;

    public:

        SpecificModelDescription(const ModelDescriptionBase &base, const T &attributes)
                : ModelDescriptionBase(base), attributes_(attributes) {}

        const SourceFiles &sourceFiles() const {
            return attributes_.sourceFiles;
        }

        std::string modelIdentifier() const {
            return attributes_.modelIdentifier;
        }

        bool canGetAndSetFMUstate() const {
            return attributes_.canGetAndSetFMUstate;
        }

        bool canSerializeFMUstate() const {
            return attributes_.canSerializeFMUstate;
        }

        bool needsExecutionTool() const {
            return attributes_.needsExecutionTool;
        }

        bool canNotUseMemoryManagementFunctions() const {
            return attributes_.canNotUseMemoryManagementFunctions;
        }

        bool canBeInstantiatedOnlyOncePerProcess() const {
            return attributes_.canBeInstantiatedOnlyOncePerProcess;
        }

        bool providesDirectionalDerivative() const {
            return attributes_.providesDirectionalDerivative;
        }

        const T &attributes() const {
            return attributes_;
        }

    };

}

#endif //FMI4CPP_SPECIFICMODELDESCRIPTION_HPP

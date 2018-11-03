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

#ifndef FMI4CPP_FMUINSTANCE_HPP
#define FMI4CPP_FMUINSTANCE_HPP

#include <vector>
#include <memory>

#include "Status.hpp"
#include "FmuVariableAccessor.hpp"

namespace fmi4cpp::fmi2 {

    typedef void *fmi2FMUstate;

    template <typename T>
    class FmuInstance: public FmuVariableAccessor {

    protected:

        double simulationTime_ = 0.0;

    public:

        virtual const double getSimulationTime() const {
            return simulationTime_;
        }

        virtual fmi4cpp::Status getLastStatus() const = 0;

        virtual std::shared_ptr<T> getModelDescription() const = 0;

        virtual bool setupExperiment(double startTime = 0.0, double stopTime = 0.0, double tolerance = 0.0) = 0;

        virtual bool enterInitializationMode() = 0;

        virtual bool exitInitializationMode() = 0;

        virtual bool reset() = 0;

        virtual bool terminate() = 0;

        virtual bool getFMUstate(void* &state) = 0;
        virtual bool setFMUstate(void* state) = 0;
        virtual bool freeFMUstate(void* &state) = 0;

        virtual bool serializeFMUstate(const fmi2FMUstate &state, std::vector<char> &serializedState) = 0;
        virtual bool deSerializeFMUstate(fmi2FMUstate &state, const std::vector<char> &serializedState) = 0;

        virtual bool getDirectionalDerivative(
                const std::vector<unsigned int> &vUnknownRef,
                const std::vector<unsigned int> &vKnownRef,
                const std::vector<double> &dvKnownRef,
                std::vector<double> &dvUnknownRef) = 0;

        virtual ~FmuInstance() = default;

    };

}

#endif //FMI4CPP_FMUINSTANCE_HPP

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
#include <string>
#include "../fmi2Functions.h"

namespace fmi4cpp::fmi2::import {

    template <typename T>
    class FmuInstance {

    protected:

        double simulationTime_ = 0.0;

        bool instantiated_ = false;
        bool terminated_ = false;

    public:

        const double getSimulationTime() const {
            return simulationTime_;
        }

        const bool isInstantiated() const {
            return instantiated_;
        }

        const bool isTerminated() const {
            return terminated_;
        }

        const fmi2ValueReference getValueReference(const std::string &name) {
            return getModelDescription().getVariableByName(name).getValueReference();
        }

        virtual std::shared_ptr<T> getModelDescription() const = 0;

        virtual void init(double start = 0, double stop = 0) = 0;

        virtual fmi2Status reset() = 0;

        virtual fmi2Status terminate() = 0;

        virtual bool canGetAndSetFMUstate() const = 0;
        virtual fmi2Status getFMUstate(fmi2FMUstate &state) = 0;
        virtual fmi2Status setFMUstate(fmi2FMUstate state) = 0;
        virtual fmi2Status freeFMUstate(fmi2FMUstate &state) = 0;

        virtual bool canSerializeFMUstate() const = 0;
        virtual fmi2Status serializeFMUstate(const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) = 0;
        virtual fmi2Status deSerializeFMUstate(fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) = 0;

        virtual bool providesDirectionalDerivative() const = 0;
        virtual fmi2Status getDirectionalDerivative(
                const std::vector<fmi2ValueReference> &vUnkownRef,
                const std::vector<fmi2ValueReference> &vKnownRef,
                const std::vector<fmi2Real> &dvKnownRef,
                std::vector<fmi2Real> &dvUnknownRef) const = 0;


        virtual fmi2Status readInteger(fmi2ValueReference vr, fmi2Integer &ref) const = 0;
        virtual fmi2Status readInteger(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) const = 0;

        virtual fmi2Status readReal(fmi2ValueReference vr, fmi2Real &ref) const = 0;
        virtual fmi2Status readReal(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) const = 0;

        virtual fmi2Status readString(fmi2ValueReference vr, fmi2String &ref) const = 0;
        virtual fmi2Status readString(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) const = 0;

        virtual fmi2Status readBoolean(fmi2ValueReference vr, fmi2Boolean &ref) const = 0;
        virtual fmi2Status readBoolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) const = 0;

        virtual fmi2Status writeInteger(fmi2ValueReference vr, fmi2Integer value) = 0;
        virtual fmi2Status writeInteger(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer > &values) = 0;

        virtual fmi2Status writeReal(fmi2ValueReference vr, fmi2Real value) = 0;
        virtual fmi2Status writeReal(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real > &values) = 0;

        virtual fmi2Status writeString(fmi2ValueReference vr, fmi2String value) = 0;
        virtual fmi2Status writeString(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String > &values) = 0;

        virtual fmi2Status writeBoolean(fmi2ValueReference vr, fmi2Boolean value) = 0;
        virtual fmi2Status writeBoolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean > &values) = 0;

        virtual ~FmuInstance() = default;

    };

}

#endif //FMI4CPP_FMUINSTANCE_HPP

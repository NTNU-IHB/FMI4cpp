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
#include "../xml/ModelDescription.hpp"
#include "../fmi2Functions.h"

using std::vector;

namespace fmi4cpp::fmi2::import {

    class FmuInstance {

    protected:

        double simulationTime_ = 0.0;

        bool instantiated_ = false;
        bool terminated_ = false;

    public:

        virtual const double getSimulationTime() const {
            return simulationTime_;
        }

        virtual const bool isInstantiated() const {
            return instantiated_;
        }

        virtual const bool isTerminated() const {
            return terminated_;
        }

        virtual const fmi2ValueReference getValueReference(const std::string &name) const {
            return getModelDescription().getVariableByName(name).getValueReference();
        }

        virtual const xml::ModelDescription &getModelDescription() const = 0;

        virtual void init(const double start = 0, const double stop = 0) = 0;

        virtual fmi2Status reset() = 0;

        virtual fmi2Status terminate() = 0;

        virtual bool canGetAndSetFMUstate() const = 0;

        virtual fmi2Status getFMUstate(fmi2FMUstate &state) = 0;

        virtual fmi2Status setFMUstate(const fmi2FMUstate state) = 0;

        virtual fmi2Status freeFMUstate(fmi2FMUstate &state) = 0;

        virtual bool canSerializeFMUstate() const = 0;

        virtual fmi2Status serializeFMUstate(const fmi2FMUstate &state, vector<fmi2Byte> &serializedState) = 0;

        virtual fmi2Status deSerializeFMUstate(fmi2FMUstate &state, const vector<fmi2Byte> &serializedState) = 0;

        virtual bool providesDirectionalDerivative() const = 0;

        virtual fmi2Status getDirectionalDerivative(
                const vector<fmi2ValueReference> &vUnkownRef,
                const vector<fmi2ValueReference> &vKnownRef,
                const vector<fmi2Real> &dvKnownRef,
                vector<fmi2Real> &dvUnknownRef) const = 0;


        virtual fmi2Status readInteger(const string &name, fmi2Integer &ref) const {
            const auto vr = getModelDescription().getVariableByName(name).getValueReference();
            return readInteger(vr, ref);
        }

        virtual fmi2Status readInteger(const fmi2ValueReference vr, fmi2Integer &ref) const = 0;

        virtual fmi2Status readInteger(const vector<fmi2ValueReference> &vr, vector<fmi2Integer> &ref) const = 0;

        virtual fmi2Status readReal(const string &name, fmi2Real &ref) const {
            const auto vr = getModelDescription().getVariableByName(name).getValueReference();
            return readReal(vr, ref);
        }

        virtual fmi2Status readReal(const fmi2ValueReference vr, fmi2Real &ref) const = 0;

        virtual fmi2Status readReal(const vector<fmi2ValueReference> &vr, vector<fmi2Real> &ref) const = 0;

        virtual fmi2Status readString(const string &name, fmi2String &ref) const {
            const auto vr = getModelDescription().getVariableByName(name).getValueReference();
            return readString(vr, ref);
        }

        virtual fmi2Status readString(const fmi2ValueReference vr, fmi2String &ref) const = 0;

        virtual fmi2Status readString(const vector<fmi2ValueReference> &vr, vector<fmi2String> &ref) const = 0;

        virtual fmi2Status readBoolean(const string &name, fmi2Boolean &ref) const {
            const auto vr = getModelDescription().getVariableByName(name).getValueReference();
            return readBoolean(vr, ref);
        }

        virtual fmi2Status readBoolean(const fmi2ValueReference vr, fmi2Boolean &ref) const = 0;

        virtual fmi2Status readBoolean(const vector<fmi2ValueReference> &vr, vector<fmi2Boolean> &ref) const = 0;

        virtual ~FmuInstance() {};

    };

}

#endif //FMI4CPP_FMUINSTANCE_HPP

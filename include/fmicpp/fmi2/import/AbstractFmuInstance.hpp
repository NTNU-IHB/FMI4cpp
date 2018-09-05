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

#ifndef FMICPP_ABSTRACTFMUINSTANCE_HPP
#define FMICPP_ABSTRACTFMUINSTANCE_HPP

#include <type_traits>
#include "FmuInstance.hpp"
#include "FmiLibrary.hpp"
#include "../xml/ModelDescription.hpp"

namespace {
    void checkStatus(fmi2Status status, string function_name) {
        if (status != fmi2OK) {
            throw std::runtime_error(function_name + " failed with status: " + std::to_string(status));
        }
    }
}

namespace fmicpp::fmi2::import {

    template<typename T, typename U>
    class AbstractFmuInstance: virtual public FmuInstance {

        static_assert(std::is_base_of<FmiLibrary, T>::value, "T must derive from FmiLibrary");
        static_assert(std::is_base_of<xml::SpecificModelDescription, U>::value, "U must derive from SpecificModelDescription");

    protected:

        fmi2Component c_;
        std::shared_ptr<T> library_;
        const std::shared_ptr<U> modelDescription_;

    public:

        AbstractFmuInstance(const shared_ptr<U> modelDescription, const shared_ptr<T> library)
                : modelDescription_(modelDescription), library_(library) {}


        const U &getModelDescription() const override {
            return *modelDescription_;
        }

        void init(const double start = 0, const double stop = 0) override {

            if (!instantiated_) {

                c_ = library_->instantiate(modelDescription_->modelIdentifier,
                        fmi2CoSimulation, modelDescription_->guid, "", false, false);

                checkStatus(library_->setupExperiment(c_, false, 1E-4, start, stop), "setupExperiment");

                checkStatus(library_->enterInitializationMode(c_), "enterInitializationMode");
                checkStatus(library_->exitInitializationMode(c_), "exitInitializationMode");

                instantiated_ = true;
                simulationTime_ = start;
            }

        }

        fmi2Status reset() override {
            return library_->reset(c_);
        }

        fmi2Status terminate() override {
            if (!terminated_) {
                terminated_ = true;
                return library_->terminate(c_);
            }
            return fmi2OK;
        }

        bool canGetAndSetFMUstate() const override {
            return modelDescription_->canGetAndSetFMUstate;
        }

        fmi2Status getFMUstate(fmi2FMUstate &state) override {
            return library_->getFMUstate(c_, state);
        }

        fmi2Status setFMUstate(const fmi2FMUstate state) override {
            return library_->setFMUstate(c_, state);
        }

        fmi2Status freeFMUstate(fmi2FMUstate &state) override {
            return library_->freeFMUstate(c_, state);
        }

        bool canSerializeFmuState() const override {
            return modelDescription_->canSerializeFMUstate;
        }

        fmi2Status serializeFMUstate(const fmi2FMUstate &state, vector<fmi2Byte> &serializedState) override {
            return library_->serializeFMUstate(c_, state, serializedState);
        }

        fmi2Status
        deSerializeFMUstate(fmi2FMUstate &state, const vector<fmi2Byte> &serializedState) override {
            return library_->deSerializeFMUstate(c_, state, serializedState);
        }

        bool providesDirectionalDerivative() const override {
            return modelDescription_->providesDirectionalDerivative;
        }

        fmi2Status getDirectionalDerivative(
                const vector<fmi2ValueReference> &vUnkownRef, const vector<fmi2ValueReference> &vKnownRef,
                const vector<fmi2Real> &dvKnownRef, vector<fmi2Real> &dvUnknownRef) const override {
            return library_->getDirectionalDerivative(c_, vUnkownRef, vKnownRef, dvKnownRef, dvUnknownRef);
        }

        fmi2Status readInteger(const fmi2ValueReference vr, fmi2Integer &ref) const override {
            return library_->readInteger(c_, vr, ref);
        }

        fmi2Status readInteger(const vector<fmi2ValueReference> &vr, vector<fmi2Integer> &ref) const override {
            return library_->readInteger(c_,vr, ref);
        }

        fmi2Status readReal(const fmi2ValueReference vr, fmi2Real &ref) const override {
            return library_->readReal(c_,vr, ref);
        }

        fmi2Status readReal(const vector<fmi2ValueReference> &vr, vector<fmi2Real> &ref) const override {
            return library_->readReal(c_, vr, ref);
        }

        fmi2Status readString(const fmi2ValueReference vr, fmi2String &ref) const override {
            return library_->readString(c_, vr, ref);
        }

        fmi2Status readString(const vector<fmi2ValueReference> &vr, vector<fmi2String> &ref) const override {
            return library_->readString(c_, vr, ref);
        }

        fmi2Status readBoolean(const fmi2ValueReference vr, fmi2Boolean &ref) const override {
            return library_->readBoolean(c_, vr, ref);
        }

        fmi2Status readBoolean(const vector<fmi2ValueReference> &vr, vector<fmi2Boolean> &ref) const override {
            return library_->readBoolean(c_, vr, ref);
        }

        ~AbstractFmuInstance() {
            terminate();
            if (c_) {
                library_->freeInstance(c_);
                c_ = nullptr;
            }
        }

    };

}

#endif //FMICPP_ABSTRACTFMUINSTANCE_HPP

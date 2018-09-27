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

#ifndef FMI4CPP_ABSTRACTFMUINSTANCE_HPP
#define FMI4CPP_ABSTRACTFMUINSTANCE_HPP

#if FMI4CPP_DEBUG_LOGGING_ENABLED
#include <iostream>
#endif

#include <type_traits>
#include "FmuInstance.hpp"
#include "FmiLibrary.hpp"
#include "../enumsToString.hpp"
#include "../xml/SpecificModelDescription.hpp"

namespace {

    void checkStatus(const fmi2Status status, const string &function_name) {
        if (status != fmi2OK) {
            throw std::runtime_error(function_name + " failed with status: " + to_string(status));
        }
    }

    bool assignStart(const fmi4cpp::fmi2::xml::ScalarVariable &v, const fmi4cpp::fmi2::import::FmuInstance &instance) {

        if (v.isIntegerVariable()) {
            IntegerVariable v = v.asIntegerVariable();
            auto start = v.getStart();
            if (v.hasStartChanged() && start) {
                instance.writeInteger(v.getValueReference(), start.value());
                return true;
            }
        } else if (v.isRealVariable()) {
            RealVariable v = v.asRealVariable();
            auto start = v.getStart();
            if (v.hasStartChanged() && start) {
                instance.writeReal(v.getValueReference(), start.value());
                return true;
            }
        } else if (v.isStringVariable()) {
            StringVariable v = v.asStringVariable();
            auto start = v.getStart();
            if (v.hasStartChanged() && start) {
                instance.writeString(v.getValueReference(), start.value().c_str());
                return true;
            }
        } else if (v.isBooleanVariable()) {
            BooleanVariable v = v.asBooleanVariable();
            auto start = v.getStart();
            if (v.hasStartChanged() && start) {
                instance.writeBoolean(v.getValueReference(), start.value());
                return true;
            }
        } else if (v.isEnumerationVariable()) {
            EnumerationVariable v = v.asEnumerationVariable();
            auto start = v.getStart();
            if (v.hasStartChanged() && start) {
                instance.writeInteger(v.getValueReference(), start.value());
                return true;
            }
        } else {
            return false;
        }

    }

}

namespace fmi4cpp::fmi2::import {

    template<typename T, typename U>
    class AbstractFmuInstance : virtual public FmuInstance {

        static_assert(std::is_base_of<FmiLibrary, T>::value, "T must derive from FmiLibrary");
//        static_assert(std::is_base_of<xml::SpecificModelDescription<xml::FmuTypeAttributes>, U>::value,
//                      "U must derive from SpecificModelDescription");

    private:
        bool instanceFreed;

    protected:

        fmi2Component c_;
        U modelDescription_;
        std::shared_ptr<T> library_;

    public:

        AbstractFmuInstance(const fmi2Component c, const shared_ptr<T> &library, U &modelDescription)
                : c_(c), library_(library), modelDescription_(modelDescription) {}


        U &getModelDescription() override {
            return modelDescription_;
        }

        fmi2Status setDebugLogging(const bool loggingOn, const vector<const char *> categories) const {
            return library_->setDebugLogging(c_, loggingOn, categories);
        }

        void init(const double start = 0, const double stop = 0) override {

            if (!instantiated_) {

                unsigned int count = 0;
                for (ScalarVariable &v: modelDescription_.getModelVariables()) {
                    if (v.getVariability() != fmi2Variability::constant && v.getInitial() == fmi2Initial::exact ||
                        v.getInitial() == fmi2Initial::approx) {
                        if (assignStart(v, *this)) {
                            count++;
                        }
                    }
                }

#if FMI4CPP_DEBUG_LOGGING_ENABLED
                if (count > 0) {
                    std::cout << "Assigned modified start values to" << count << " variables with variability != constant and initial == exact or approx" << std::endl;
                }
#endif

                checkStatus(library_->setupExperiment(c_, false, 1E-4, start, stop), "setupExperiment");

                checkStatus(library_->enterInitializationMode(c_), "enterInitializationMode");

                count = 0;
                for (ScalarVariable &v: modelDescription_.getModelVariables()) {
                    if (v.getVariability() != fmi2Variability::constant && v.getInitial() == fmi2Initial::exact ||
                        v.getCausality() == fmi2Causality::input) {
                        if (assignStart(v, *this)) {
                            count++;
                        }
                    }
                }

#if FMI4CPP_DEBUG_LOGGING_ENABLED
                if (count > 0) {
                    std::cout << "Assigned modified start values to" << count << " variables with variability != constant and initial == exact or causality == input" << std::endl;
                }
#endif

                checkStatus(library_->exitInitializationMode(c_), "exitInitializationMode");

                instantiated_ = true;
                simulationTime_ = start;

            }

        }

        fmi2Status reset() override {
            return library_->reset(c_);
        }

        fmi2Status terminate() override {
            return terminate(true);
        }

        fmi2Status terminate(bool freeInstance) {
            if (!terminated_) {
                terminated_ = true;
                fmi2Status status = library_->terminate(c_);
                if (freeInstance) {
                    this->freeInstance();
                }
                return status;
            }
            return fmi2OK;
        }

        void freeInstance() {
            if (!instanceFreed) {
                instanceFreed = true;
                library_->freeInstance(c_);
                c_ = nullptr;
            }
        }

        bool canGetAndSetFMUstate() const override {
            return modelDescription_.canGetAndSetFMUstate();
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

        bool canSerializeFMUstate() const override {
            return modelDescription_.canSerializeFMUstate();
        }

        fmi2Status getSerializedFMUstateSize(const fmi2FMUstate state, size_t &size) const {
            return library_->getSerializedFMUstateSize(c_, state, size);
        }

        fmi2Status serializeFMUstate(const fmi2FMUstate &state, vector<fmi2Byte> &serializedState) override {
            return library_->serializeFMUstate(c_, state, serializedState);
        }

        fmi2Status
        deSerializeFMUstate(fmi2FMUstate &state, const vector<fmi2Byte> &serializedState) override {
            return library_->deSerializeFMUstate(c_, state, serializedState);
        }

        bool providesDirectionalDerivative() const override {
            return modelDescription_.providesDirectionalDerivative();
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
            return library_->readInteger(c_, vr, ref);
        }

        fmi2Status readReal(const fmi2ValueReference vr, fmi2Real &ref) const override {
            return library_->readReal(c_, vr, ref);
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

        fmi2Status writeInteger(const fmi2ValueReference vr, const fmi2Integer value) const override {
            return library_->writeInteger(c_, vr, value);
        }

        fmi2Status
        writeInteger(const vector<fmi2ValueReference> &vr, const vector<fmi2Integer> &values) const override {
            return library_->writeInteger(c_, vr, values);
        }

        fmi2Status writeReal(const fmi2ValueReference vr, const fmi2Real value) const override {
            return library_->writeReal(c_, vr, value);
        }

        fmi2Status writeReal(const vector<fmi2ValueReference> &vr, const vector<fmi2Real> &values) const override {
            return library_->writeReal(c_, vr, values);
        }

        fmi2Status writeString(const fmi2ValueReference vr, fmi2String value) const override {
            return library_->writeString(c_, vr, value);
        }

        fmi2Status writeString(const vector<fmi2ValueReference> &vr, const vector<fmi2String> &values) const override {
            return library_->writeString(c_, vr, values);
        }

        fmi2Status writeBoolean(const fmi2ValueReference vr, const fmi2Boolean value) const override {
            return library_->writeBoolean(c_, vr, value);
        }

        fmi2Status
        writeBoolean(const vector<fmi2ValueReference> &vr, const vector<fmi2Boolean> &values) const override {
            return library_->writeBoolean(c_, vr, values);
        }

        ~AbstractFmuInstance() {
            terminate();
            freeInstance();
        }

    };

}

#endif //FMI4CPP_ABSTRACTFMUINSTANCE_HPP

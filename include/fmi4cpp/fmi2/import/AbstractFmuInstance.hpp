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

#include <utility>
#include <queue>
#include <string>
#include <type_traits>
#include "FmuInstance.hpp"
#include "FmiLibrary.hpp"
#include "../enumsToString.hpp"
#include "../xml/ModelDescription.hpp"

namespace {

    void checkStatus(const fmi2Status status, const std::string &function_name) {
        if (status != fmi2OK) {
            throw std::runtime_error(function_name + " failed with status: " + to_string(status));
        }
    }

//    bool canApplyStart1(const ScalarVariable &v) {
//        return v.getVariability() != fmi2Variability::constant && v.getInitial() == fmi2Initial::exact ||
//                    v.getCausality() == fmi2Causality::input;
//    }
//    bool canApplyStart2(const ScalarVariable &v) {
//        return v.getVariability() != fmi2Variability::constant && v.getInitial() == fmi2Initial::exact ||
//                    v.getInitial() == fmi2Initial::approx;
//    }

}

namespace fmi4cpp::fmi2 {

//    class StartQueue {
//
//    private:
//
//        std::queue<std::pair<fmi2ValueReference, fmi2Integer > > integerQueue_;
//        std::queue<std::pair<fmi2ValueReference, fmi2Real > > realQueue_;
//        std::queue<std::pair<fmi2ValueReference, fmi2String > > stringQueue_;
//        std::queue<std::pair<fmi2ValueReference, fmi2Boolean > > booleanQueue_;
//
//    public:
//
//        fmi2Status putInteger(const fmi2ValueReference vr, const fmi2Integer value) {
//            integerQueue_.push(std::make_pair(vr, value));
//            return fmi2OK;
//        }
//
//        fmi2Status putReal(const fmi2ValueReference vr, const fmi2Real value) {
//            realQueue_.push(std::make_pair(vr, value));
//            return fmi2OK;
//        }
//
//        fmi2Status putString(const fmi2ValueReference vr, fmi2String value) {
//            stringQueue_.push(std::make_pair(vr, value));
//            return fmi2OK;
//        }
//
//        fmi2Status putBoolean(const fmi2ValueReference vr, const fmi2Boolean value) {
//            booleanQueue_.push(std::make_pair(vr, value));
//            return fmi2OK;
//        }
//
//        template <class T>
//        bool assignStartValues(fmi4cpp::fmi2::import::FmuInstance<T> &instance) {
//
//            while (!integerQueue_.empty()) {
//                const auto &pair = integerQueue_.front();
//                fmi2Status status = instance.writeInteger(pair.first, pair.second);
//                if (status != fmi2OK) {
//                    return false;
//                }
//                integerQueue_.pop();
//            }
//
//            while (!realQueue_.empty()) {
//                const auto &pair = realQueue_.front();
//                fmi2Status status = instance.writeReal(pair.first, pair.second);
//                if (status != fmi2OK) {
//                    return false;
//                }
//                realQueue_.pop();
//            }
//
//            while (!stringQueue_.empty()) {
//                const auto &pair = stringQueue_.front();
//                fmi2Status status = instance.writeString(pair.first, pair.second);
//                if (status != fmi2OK) {
//                    return false;
//                }
//                stringQueue_.pop();
//            }
//
//            while (!booleanQueue_.empty()) {
//                const auto &pair = booleanQueue_.front();
//                fmi2Status status = instance.writeBoolean(pair.first, pair.second);
//                if (!status == fmi2OK) {
//                    return false;
//                }
//                booleanQueue_.pop();
//            }
//
//            return true;
//
//        }
//
//    };

    template<typename T, typename U>
    class AbstractFmuInstance : public virtual FmuInstance<U> {

        static_assert(std::is_base_of<FmiLibrary, T>::value, "T must derive from FmiLibrary");
        static_assert(std::is_base_of<ModelDescriptionBase, U>::value,
                      "U must derive from ModelDescriptionBase");

    private:

        bool instanceFreed_ = false;

    protected:

        fmi2Component c_;
        const std::shared_ptr<T> library_;
        const std::shared_ptr<U> modelDescription_;

    public:

        AbstractFmuInstance(const fmi2Component c,
                            const std::shared_ptr<T> &library,
                            const std::shared_ptr<U> &modelDescription)
                : c_(c), library_(library), modelDescription_(modelDescription) {}


        std::shared_ptr<U> getModelDescription() const override {
            return modelDescription_;
        }

        fmi2Status setDebugLogging(const bool loggingOn, const std::vector<const char *> categories) const {
            return library_->setDebugLogging(c_, loggingOn, categories);
        }

        void init(const double start, const double stop) override {

            if (!this->instantiated_) {

                checkStatus(library_->setupExperiment(c_, false, 1E-4, start, stop), "setupExperiment");

                checkStatus(library_->enterInitializationMode(c_), "enterInitializationMode");
                checkStatus(library_->exitInitializationMode(c_), "exitInitializationMode");

                this->instantiated_ = true;
                this->simulationTime_ = start;

            }

        }

        fmi2Status reset() override {
            return library_->reset(c_);
        }

        fmi2Status terminate() override {
            return terminate(true);
        }

        fmi2Status terminate(bool freeInstance) {
            if (!this->terminated_) {
                this->terminated_ = true;
                fmi2Status status = library_->terminate(c_);
                if (freeInstance) {
                    this->freeInstance();
                }
                return status;
            }
            return fmi2OK;
        }

        void freeInstance() {
            if (!instanceFreed_) {
                instanceFreed_ = true;
                library_->freeInstance(c_);
                c_ = nullptr;
            }
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


        fmi2Status getSerializedFMUstateSize(const fmi2FMUstate state, size_t &size) const {
            return library_->getSerializedFMUstateSize(c_, state, size);
        }

        fmi2Status serializeFMUstate(const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) override {
            return library_->serializeFMUstate(c_, state, serializedState);
        }

        fmi2Status
        deSerializeFMUstate(fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) override {
            return library_->deSerializeFMUstate(c_, state, serializedState);
        }

        fmi2Status getDirectionalDerivative(
                const std::vector<fmi2ValueReference> &vUnkownRef, const std::vector<fmi2ValueReference> &vKnownRef,
                const std::vector<fmi2Real> &dvKnownRef, std::vector<fmi2Real> &dvUnknownRef) const override {
            return library_->getDirectionalDerivative(c_, vUnkownRef, vKnownRef, dvKnownRef, dvUnknownRef);
        }

        fmi2Status readInteger(const fmi2ValueReference vr, fmi2Integer &ref) const override {
            return library_->readInteger(c_, vr, ref);
        }

        fmi2Status
        readInteger(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) const override {
            return library_->readInteger(c_, vr, ref);
        }

        fmi2Status readReal(const fmi2ValueReference vr, fmi2Real &ref) const override {
            return library_->readReal(c_, vr, ref);
        }

        fmi2Status readReal(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) const override {
            return library_->readReal(c_, vr, ref);
        }

        fmi2Status readString(const fmi2ValueReference vr, fmi2String &ref) const override {
            return library_->readString(c_, vr, ref);
        }

        fmi2Status readString(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) const override {
            return library_->readString(c_, vr, ref);
        }

        fmi2Status readBoolean(const fmi2ValueReference vr, fmi2Boolean &ref) const override {
            return library_->readBoolean(c_, vr, ref);
        }

        fmi2Status
        readBoolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) const override {
            return library_->readBoolean(c_, vr, ref);
        }

        fmi2Status writeInteger(const fmi2ValueReference vr, const fmi2Integer value) override {
            return library_->writeInteger(c_, vr, value);
        }

        fmi2Status
        writeInteger(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer> &values) override {
            return library_->writeInteger(c_, vr, values);
        }

        fmi2Status writeReal(const fmi2ValueReference vr, const fmi2Real value) override {
            return library_->writeReal(c_, vr, value);
        }

        fmi2Status writeReal(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real> &values) override {
            return library_->writeReal(c_, vr, values);
        }

        fmi2Status writeString(const fmi2ValueReference vr, fmi2String value) override {
            return library_->writeString(c_, vr, value);
        }

        fmi2Status
        writeString(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String> &values) override {
            return library_->writeString(c_, vr, values);
        }

        fmi2Status writeBoolean(const fmi2ValueReference vr, const fmi2Boolean value) override {
            return library_->writeBoolean(c_, vr, value);
        }

        fmi2Status
        writeBoolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean> &values) override {
            return library_->writeBoolean(c_, vr, values);
        }

        ~AbstractFmuInstance() {
            terminate();
        }

    };

}

#endif //FMI4CPP_ABSTRACTFMUINSTANCE_HPP

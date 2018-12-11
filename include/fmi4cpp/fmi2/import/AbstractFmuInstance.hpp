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


#include <utility>
#include <string>
#include <type_traits>

#include "FmiLibrary.hpp"
#include "FmuInstance.hpp"
#include "status_converter.hpp"
#include "../xml/ModelDescription.hpp"

namespace fmi4cpp::fmi2 {

    template<typename T, typename U>
    class AbstractFmuInstance : public virtual FmuInstance<U> {

        static_assert(std::is_base_of<FmiLibrary, T>::value, "T must derive from FmiLibrary");
        static_assert(std::is_base_of<ModelDescriptionBase, U>::value,
                      "U must derive from ModelDescriptionBase");

    private:

        bool terminated_ = false;
        bool instanceFreed_ = false;

    protected:

        fmi2Component c_;
        const std::shared_ptr<T> library_;
        const std::shared_ptr<U> modelDescription_;

    public:

        AbstractFmuInstance(fmi2Component c,
                            const std::shared_ptr<T> &library,
                            const std::shared_ptr<U> &modelDescription)
                : c_(c), library_(library), modelDescription_(modelDescription) {}

        fmi4cpp::Status getLastStatus() const override {
            return convert(library_->getLastStatus());
        }

        std::shared_ptr<U> getModelDescription() const override {
            return modelDescription_;
        }

        bool setDebugLogging(const bool loggingOn, const std::vector<const char *> categories) const {
            return library_->setDebugLogging(c_, loggingOn, categories);
        }

        bool setupExperiment(double start = 0, double stop = 0, double tolerance = 0) override {
            this->simulationTime_ = start;
            return library_->setupExperiment(c_, tolerance, start, stop);
        }

        bool enterInitializationMode() override {
            return library_->enterInitializationMode(c_);
        }

        bool exitInitializationMode() override {
            return library_->exitInitializationMode(c_);
        }

        bool reset() override {
            return library_->reset(c_);
        }

        bool terminate() override {
            return terminate(true);
        }

        bool terminate(bool freeInstance) {
            if (!this->terminated_) {
                this->terminated_ = true;
                if (!library_->terminate(c_)) {
                    return false;
                }
                this->freeInstance();
            }
            return true;
        }

        void freeInstance() {
            if (!instanceFreed_) {
                instanceFreed_ = true;
                library_->freeInstance(c_);
                c_ = nullptr;
            }
        }

        bool getFMUstate(fmi2FMUstate &state) override {
            return library_->getFMUstate(c_, state);
        }

        bool setFMUstate(fmi2FMUstate state) override {
            return library_->setFMUstate(c_, state);
        }

        bool freeFMUstate(fmi2FMUstate &state) override {
            return library_->freeFMUstate(c_, state);
        }


        bool getSerializedFMUstateSize(fmi2FMUstate state, size_t &size) const {
            return library_->getSerializedFMUstateSize(c_, state, size);
        }

        bool serializeFMUstate(const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) override {
            return library_->serializeFMUstate(c_, state, serializedState);
        }

        bool deSerializeFMUstate(fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) override {
            return library_->deSerializeFMUstate(c_, state, serializedState);
        }

        bool getDirectionalDerivative(
                const std::vector<fmi2ValueReference> &vUnknownRef, const std::vector<fmi2ValueReference> &vKnownRef,
                const std::vector<fmi2Real> &dvKnownRef, std::vector<fmi2Real> &dvUnknownRef) override {
            return library_->getDirectionalDerivative(c_, vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
        }

        bool readInteger(const fmi2ValueReference vr, fmi2Integer &ref) override {
            return library_->readInteger(c_, vr, ref);
        }

        bool readInteger(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) override {
            return library_->readInteger(c_, vr, ref);
        }

        bool readReal(const fmi2ValueReference vr, fmi2Real &ref) override {
            return library_->readReal(c_, vr, ref);
        }

        bool readReal(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) override {
            return library_->readReal(c_, vr, ref);
        }

        bool readString(const fmi2ValueReference vr, fmi2String &ref) override {
            return library_->readString(c_, vr, ref);
        }

        bool readString(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) override {
            return library_->readString(c_, vr, ref);
        }

        bool readBoolean(const fmi2ValueReference vr, fmi2Boolean &ref) override {
            return library_->readBoolean(c_, vr, ref);
        }

        bool readBoolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) override {
            return library_->readBoolean(c_, vr, ref);
        }

        bool writeInteger(const fmi2ValueReference vr, const fmi2Integer value) override {
            return library_->writeInteger(c_, vr, value);
        }

        bool writeInteger(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer> &values) override {
            return library_->writeInteger(c_, vr, values);
        }

        bool writeReal(const fmi2ValueReference vr, const fmi2Real value) override {
            return library_->writeReal(c_, vr, value);
        }

        bool writeReal(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real> &values) override {
            return library_->writeReal(c_, vr, values);
        }

        bool writeString(const fmi2ValueReference vr, fmi2String value) override {
            return library_->writeString(c_, vr, value);
        }

        bool writeString(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String> &values) override {
            return library_->writeString(c_, vr, values);
        }

        bool writeBoolean(const fmi2ValueReference vr, const fmi2Boolean value) override {
            return library_->writeBoolean(c_, vr, value);
        }

        bool writeBoolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean> &values) override {
            return library_->writeBoolean(c_, vr, values);
        }

        ~AbstractFmuInstance() {
            terminate();
        }

    };

}

#endif //FMI4CPP_ABSTRACTFMUINSTANCE_HPP

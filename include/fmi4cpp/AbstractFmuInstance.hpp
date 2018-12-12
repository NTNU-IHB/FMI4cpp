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

#include <memory>
#include <string>

#include "types.hpp"
#include "Status.hpp"
#include "FmuInstance.hpp"

namespace fmi4cpp {

    template<typename FmiLibrary, typename ModelDescription>
    class AbstractFmuInstance : public virtual FmuInstance<ModelDescription> {

    private:

        bool terminated_ = false;
        bool instanceFreed_ = false;

    protected:

        fmi2Component c_;
        const std::shared_ptr<FmiLibrary> library_;
        const std::shared_ptr<ModelDescription> modelDescription_;

    public:

        AbstractFmuInstance(fmi2Component c,
                            const std::shared_ptr<FmiLibrary> &library,
                            const std::shared_ptr<ModelDescription> &modelDescription)
                : c_(c), library_(library), modelDescription_(modelDescription) {}

        std::shared_ptr<ModelDescription> getModelDescription() const override {
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

        bool getFMUstate(fmi4cppFMUstate &state) override {
            return library_->getFMUstate(c_, state);
        }

        bool setFMUstate(fmi4cppFMUstate state) override {
            return library_->setFMUstate(c_, state);
        }

        bool freeFMUstate(fmi4cppFMUstate &state) override {
            return library_->freeFMUstate(c_, state);
        }


        bool getSerializedFMUstateSize(fmi4cppFMUstate state, size_t &size) const {
            return library_->getSerializedFMUstateSize(c_, state, size);
        }

        bool serializeFMUstate(const fmi4cppFMUstate &state, std::vector<fmi4cppByte> &serializedState) override {
            return library_->serializeFMUstate(c_, state, serializedState);
        }

        bool deSerializeFMUstate(fmi4cppFMUstate &state, const std::vector<fmi4cppByte> &serializedState) override {
            return library_->deSerializeFMUstate(c_, state, serializedState);
        }

        bool getDirectionalDerivative(
                const std::vector<fmi4cppValueReference> &vUnknownRef, const std::vector<fmi4cppValueReference> &vKnownRef,
                const std::vector<fmi4cppReal> &dvKnownRef, std::vector<fmi4cppReal> &dvUnknownRef) override {
            return library_->getDirectionalDerivative(c_, vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
        }

        bool readInteger(const fmi4cppValueReference vr, fmi4cppInteger &ref) override {
            return library_->readInteger(c_, vr, ref);
        }

        bool readInteger(const std::vector<fmi4cppValueReference> &vr, std::vector<fmi4cppInteger> &ref) override {
            return library_->readInteger(c_, vr, ref);
        }

        bool readReal(const fmi4cppValueReference vr, fmi4cppReal &ref) override {
            return library_->readReal(c_, vr, ref);
        }

        bool readReal(const std::vector<fmi4cppValueReference> &vr, std::vector<fmi4cppReal> &ref) override {
            return library_->readReal(c_, vr, ref);
        }

        bool readString(const fmi4cppValueReference vr, fmi4cppString &ref) override {
            return library_->readString(c_, vr, ref);
        }

        bool readString(const std::vector<fmi4cppValueReference> &vr, std::vector<fmi4cppString> &ref) override {
            return library_->readString(c_, vr, ref);
        }

        bool readBoolean(const fmi4cppValueReference vr, fmi4cppBoolean &ref) override {
            return library_->readBoolean(c_, vr, ref);
        }

        bool readBoolean(const std::vector<fmi4cppValueReference> &vr, std::vector<fmi4cppBoolean> &ref) override {
            return library_->readBoolean(c_, vr, ref);
        }

        bool writeInteger(const fmi4cppValueReference vr, const fmi4cppInteger value) override {
            return library_->writeInteger(c_, vr, value);
        }

        bool writeInteger(const std::vector<fmi4cppValueReference> &vr, const std::vector<fmi4cppInteger> &values) override {
            return library_->writeInteger(c_, vr, values);
        }

        bool writeReal(const fmi4cppValueReference vr, const fmi4cppReal value) override {
            return library_->writeReal(c_, vr, value);
        }

        bool writeReal(const std::vector<fmi4cppValueReference> &vr, const std::vector<fmi4cppReal> &values) override {
            return library_->writeReal(c_, vr, values);
        }

        bool writeString(const fmi4cppValueReference vr, fmi4cppString value) override {
            return library_->writeString(c_, vr, value);
        }

        bool writeString(const std::vector<fmi4cppValueReference> &vr, const std::vector<fmi4cppString> &values) override {
            return library_->writeString(c_, vr, values);
        }

        bool writeBoolean(const fmi4cppValueReference vr, const fmi4cppBoolean value) override {
            return library_->writeBoolean(c_, vr, value);
        }

        bool writeBoolean(const std::vector<fmi4cppValueReference> &vr, const std::vector<fmi4cppBoolean> &values) override {
            return library_->writeBoolean(c_, vr, values);
        }

        ~AbstractFmuInstance() {
            terminate();
        }

    };

}

#endif //FMI4CPP_ABSTRACTFMUINSTANCE_HPP

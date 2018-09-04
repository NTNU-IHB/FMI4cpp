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

#include <iostream>
#include <fmicpp/fmi2/import/AbstractFmuInstance.hpp>

using namespace std;
using namespace fmicpp::fmi2::import;

namespace {
    void checkStatus(fmi2Status status, string function_name) {
        if (status != fmi2OK) {
            throw runtime_error(function_name + " failed with status: " + to_string(status));
        }
    }
}

template<typename T, typename U>
AbstractFmuInstance<T, U>::AbstractFmuInstance(const shared_ptr<U> modelDescription, const shared_ptr<T> library)
    : modelDescription_(modelDescription), library_(library) {}

template<typename T, typename U>
void AbstractFmuInstance<T, U>::init(const double start, const double stop) {

    if (!instantiated_) {

        cout << "hello init" << endl;

        c_ = library_->instantiate(modelDescription_->modelIdentifier, fmi2CoSimulation, modelDescription_->guid, "", false, false);

        checkStatus(library_->setupExperiment(c_, false, 1E-4, start, stop), "setupExperiment");

        checkStatus(library_->enterInitializationMode(c_), "enterInitializationMode");
        checkStatus(library_->exitInitializationMode(c_), "exitInitializationMode");

        instantiated_ = true;
        simulationTime_ = start;
    }

}

template<typename T, typename U>
const U &AbstractFmuInstance<T, U>::getModelDescription() const {
    return *modelDescription_;
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::reset() {
    return library_->reset(c_);
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::terminate() {
    if (!terminated_) {
        terminated_ = true;
        return library_->terminate(c_);
    }
   return fmi2OK;
}

template<typename T, typename U>
bool AbstractFmuInstance<T, U>::canGetAndSetFMUstate() const {
    return modelDescription_->canGetAndSetFMUstate;
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::getFMUstate(fmi2FMUstate &state) {
    return library_->getFMUstate(c_, state);
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::setFMUstate(const fmi2FMUstate state) {
    return library_->setFMUstate(c_, state);
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::freeFMUstate(fmi2FMUstate &state) {
    return library_->freeFMUstate(c_, state);
}

template<typename T, typename U>
bool AbstractFmuInstance<T, U>::canSerializeFmuState() const {
    return modelDescription_->canSerializeFMUstate;
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::serializeFMUstate(vector<fmi2Byte> &serializedState) {
    return fmi2Error;
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::deSerializeFMUstate(const vector<fmi2Byte> &serializedState, fmi2FMUstate &state) {
    return fmi2Error;
}

template<typename T, typename U>
bool AbstractFmuInstance<T, U>::providesDirectionalDerivative() const {
    return modelDescription_->providesDirectionalDerivative;
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::getDirectionalDerivative(const vector<fmi2ValueReference> &vUnkownRef,
                                                            const vector<fmi2ValueReference> &vKnownRef,
                                                            const vector<fmi2Real> &dvKnownRef,
                                                            vector<fmi2Real> &dvUnknownRef) const{
    return fmi2Error;
}


template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::readInteger(const fmi2ValueReference vr, fmi2Integer &ref) const {
    return library_->readInteger(c_, vr, ref);
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::readInteger(const vector<fmi2ValueReference> &vr, vector<fmi2Integer> &ref) const {
    return library_->readInteger(c_,vr, ref);
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::readReal(const fmi2ValueReference vr, fmi2Real &ref) const {
    return library_->readReal(c_,vr, ref);
}

template<typename T, typename U>
fmi2Status AbstractFmuInstance<T, U>::readReal(const vector<fmi2ValueReference> &vr, vector<fmi2Real> &ref) const {
    return library_->readReal(c_, vr, ref);
}

template<typename T, typename U>
AbstractFmuInstance<T, U>::~AbstractFmuInstance() {
    terminate();
    if (c_) {
        library_->freeInstance(c_);
        c_ = nullptr;
    }
}

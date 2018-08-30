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

#include <fmicpp/fmi2/import/AbstractFmuInstance.hpp>

using namespace std;
using namespace fmicpp::fmi2::import;

namespace {

    void checkStatus(fmi2Status status, string function_name) {
        if (status != fmi2OK) {
            throw runtime_error(function_name + " failed with status: ");
        }
    }

}

template<typename T>
AbstractFmuInstance<T>::AbstractFmuInstance(const shared_ptr<T> library):library(library) {}

template<typename T>
void AbstractFmuInstance<T>::init(double start, double stop) {

    if (!instantiated) {

        checkStatus(library->setupExperiment(false, 1E-4, start, stop), "setupExperiment");

        checkStatus(library->enterInitializationMode(), "enterInitializationMode");
        checkStatus(library->exitInitializationMode(), "exitInitializationMode");

        instantiated = true;
        simulationTime = start;
    }

}



template<typename T>
fmi2Status AbstractFmuInstance<T>::reset() {
    return library->reset();
}

template<typename T>
fmi2Status AbstractFmuInstance<T>::terminate() {
    if (!terminated) {
        terminated = true;
        return library->terminate();
    }
   return fmi2OK;
}

template<typename T>
bool AbstractFmuInstance<T>::canGetAndSetFMUstate() const {
    return false;
}

template<typename T>
fmi2Status AbstractFmuInstance<T>::getFMUstate(fmi2FMUstate &state) {
    return fmi2Error;
}

template<typename T>
fmi2Status AbstractFmuInstance<T>::setFMUstate(const fmi2FMUstate state) {
    return fmi2Error;
}

template<typename T>
fmi2Status AbstractFmuInstance<T>::freeFMUstate(fmi2FMUstate &state) {
    return fmi2Error;
}

template<typename T>
bool AbstractFmuInstance<T>::canSerializeFmuState() const {
    return false;
}

template<typename T>
fmi2Status AbstractFmuInstance<T>::serializeFMUstate(vector<fmi2Byte> &serializedState) {
    return fmi2Error;
}

template<typename T>
fmi2Status AbstractFmuInstance<T>::deSerializeFMUstate(const vector<fmi2Byte> &serializedState, fmi2FMUstate &state) {
    return fmi2Error;
}

template<typename T>
bool AbstractFmuInstance<T>::providesDirectionalDerivative() const {
    return false;
}

template<typename T>
fmi2Status AbstractFmuInstance<T>::getDirectionalDerivative(const vector<fmi2ValueReference> &vUnkownRef,
                                                            const vector<fmi2ValueReference> &vKnownRef,
                                                            const vector<fmi2Real> &dvKnownRef,
                                                            vector<fmi2Real> &dvUnknownRef) {
    return fmi2Error;
}


template<typename T>
AbstractFmuInstance<T>::~AbstractFmuInstance() {
    terminate();
    library->freeInstance();
}


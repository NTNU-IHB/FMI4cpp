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

#include <fmi4cpp/fmi2/import/CoSimulationSlave.hpp>

using namespace fmi4cpp::fmi2;

CoSimulationSlave::CoSimulationSlave(const fmi2Component c,
                                     const std::shared_ptr<CoSimulationLibrary> &library,
                                     const std::shared_ptr<CoSimulationModelDescription> &modelDescription)
        : AbstractFmuInstance<CoSimulationLibrary, CoSimulationModelDescription>(c, library, modelDescription) {}

bool CoSimulationSlave::doStep(const double stepSize) {
    if (library_->doStep(c_, simulationTime_, stepSize, false)) {
        simulationTime_ += stepSize;
        return true;
    }
    return false;
}

bool CoSimulationSlave::cancelStep() {
    return library_->cancelStep(c_);
}

std::shared_ptr<CoSimulationModelDescription> CoSimulationSlave::getModelDescription() const {
    return AbstractFmuInstance::getModelDescription();
}


fmi4cpp::Status CoSimulationSlave::getLastStatus() const {
    return AbstractFmuInstance::getLastStatus();
}

bool CoSimulationSlave::setupExperiment(double start, double stop, double tolerance) {
    return AbstractFmuInstance::setupExperiment(start, stop, tolerance);
}

bool CoSimulationSlave::enterInitializationMode() {
    return AbstractFmuInstance::enterInitializationMode();
}

bool CoSimulationSlave::exitInitializationMode() {
    return AbstractFmuInstance::exitInitializationMode();
}

bool CoSimulationSlave::reset() {
    return AbstractFmuInstance::reset();
}

bool CoSimulationSlave::terminate() {
    return AbstractFmuInstance::terminate();
}

bool CoSimulationSlave::readInteger(unsigned int vr, int &ref) {
    return AbstractFmuInstance::readInteger(vr, ref);
}

bool CoSimulationSlave::readInteger(const std::vector<unsigned int> &vr, std::vector<int> &ref) {
    return AbstractFmuInstance::readInteger(vr, ref);
}

bool CoSimulationSlave::readReal(unsigned int vr, double &ref) {
    return AbstractFmuInstance::readReal(vr, ref);
}

bool CoSimulationSlave::readReal(const std::vector<unsigned int> &vr, std::vector<double> &ref) {
    return AbstractFmuInstance::readReal(vr, ref);
}

bool CoSimulationSlave::readString(unsigned int vr, const char *&ref) {
    return AbstractFmuInstance::readString(vr, ref);
}

bool CoSimulationSlave::readString(const std::vector<unsigned int> &vr, std::vector<const char *> &ref) {
    return AbstractFmuInstance::readString(vr, ref);
}

bool CoSimulationSlave::readBoolean(unsigned int vr, int &ref) {
    return AbstractFmuInstance::readBoolean(vr, ref);
}

bool CoSimulationSlave::readBoolean(const std::vector<unsigned int> &vr, std::vector<int> &ref) {
    return AbstractFmuInstance::readBoolean(vr, ref);
}

bool CoSimulationSlave::writeInteger(unsigned int vr, int value) {
    return AbstractFmuInstance::writeInteger(vr, value);
}

bool CoSimulationSlave::writeInteger(const std::vector<unsigned int> &vr, const std::vector<int> &values) {
    return AbstractFmuInstance::writeInteger(vr, values);
}

bool CoSimulationSlave::writeReal(unsigned int vr, double value) {
    return AbstractFmuInstance::writeReal(vr, value);
}

bool CoSimulationSlave::writeReal(const std::vector<unsigned int> &vr, const std::vector<double> &values) {
    return AbstractFmuInstance::writeReal(vr, values);
}

bool CoSimulationSlave::writeString(unsigned int vr, const char *value) {
    return AbstractFmuInstance::writeString(vr, value);
}

bool CoSimulationSlave::writeString(const std::vector<unsigned int> &vr, const std::vector<const char *> &values) {
    return AbstractFmuInstance::writeString(vr, values);
}

bool CoSimulationSlave::writeBoolean(unsigned int vr, int value) {
    return AbstractFmuInstance::writeBoolean(vr, value);
}

bool CoSimulationSlave::writeBoolean(const std::vector<unsigned int> &vr, const std::vector<int> &values) {
    return AbstractFmuInstance::writeBoolean(vr, values);
}


bool CoSimulationSlave::getFMUstate(void *&state) {
    return AbstractFmuInstance::getFMUstate(state);
}

bool CoSimulationSlave::setFMUstate(void *state) {
    return AbstractFmuInstance::setFMUstate(state);
}

bool CoSimulationSlave::freeFMUstate(void *&state) {
    return AbstractFmuInstance::freeFMUstate(state);
}

bool CoSimulationSlave::serializeFMUstate(const fmi2FMUstate &state, std::vector<char> &serializedState) {
    return AbstractFmuInstance::serializeFMUstate(state, serializedState);
}

bool CoSimulationSlave::deSerializeFMUstate(fmi2FMUstate &state, const std::vector<char> &serializedState) {
    return AbstractFmuInstance::deSerializeFMUstate(state, serializedState);
}

bool CoSimulationSlave::getDirectionalDerivative(const std::vector<unsigned int> &vUnknownRef,
                                                 const std::vector<unsigned int> &vKnownRef,
                                                 const std::vector<double> &dvKnownRef,
                                                 std::vector<double> &dvUnknownRef) {
    return AbstractFmuInstance::getDirectionalDerivative(vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}



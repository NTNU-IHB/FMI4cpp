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

#include "fmi2StatusConverter.hpp"
#include <fmi4cpp/fmi2/import/fmi2CoSimulationSlave.hpp>

using namespace fmi4cpp::fmi2;

fmi2CoSimulationSlave::fmi2CoSimulationSlave(const fmi2Component c,
                                     const std::shared_ptr<fmi4cpp::FmuResource> &resource,
                                     const std::shared_ptr<fmi2CoSimulationLibrary> &library,
                                     const std::shared_ptr<const CoSimulationModelDescription> &modelDescription)
        : AbstractFmuInstance<fmi2CoSimulationLibrary, CoSimulationModelDescription>(c, resource, library, modelDescription) {}

fmi4cpp::Status fmi2CoSimulationSlave::getLastStatus() const {
    return convert(library_->getLastStatus());
}

bool fmi2CoSimulationSlave::doStep(const double stepSize) {
    if (library_->doStep(c_, simulationTime_, stepSize, false)) {
        simulationTime_ += stepSize;
        return true;
    }
    return false;
}

bool fmi2CoSimulationSlave::cancelStep() {
    return library_->cancelStep(c_);
}

std::shared_ptr<const CoSimulationModelDescription> fmi2CoSimulationSlave::getModelDescription() const {
    return AbstractFmuInstance::getModelDescription();
}

bool fmi2CoSimulationSlave::setupExperiment(double start, double stop, double tolerance) {
    return AbstractFmuInstance::setupExperiment(start, stop, tolerance);
}

bool fmi2CoSimulationSlave::enterInitializationMode() {
    return AbstractFmuInstance::enterInitializationMode();
}

bool fmi2CoSimulationSlave::exitInitializationMode() {
    return AbstractFmuInstance::exitInitializationMode();
}

bool fmi2CoSimulationSlave::reset() {
    return AbstractFmuInstance::reset();
}

bool fmi2CoSimulationSlave::terminate() {
    return AbstractFmuInstance::terminate();
}

bool fmi2CoSimulationSlave::readInteger(unsigned int vr, int &ref) {
    return AbstractFmuInstance::readInteger(vr, ref);
}

bool fmi2CoSimulationSlave::readInteger(const std::vector<unsigned int> &vr, std::vector<int> &ref) {
    return AbstractFmuInstance::readInteger(vr, ref);
}

bool fmi2CoSimulationSlave::readReal(unsigned int vr, double &ref) {
    return AbstractFmuInstance::readReal(vr, ref);
}

bool fmi2CoSimulationSlave::readReal(const std::vector<unsigned int> &vr, std::vector<double> &ref) {
    return AbstractFmuInstance::readReal(vr, ref);
}

bool fmi2CoSimulationSlave::readString(unsigned int vr, const char *&ref) {
    return AbstractFmuInstance::readString(vr, ref);
}

bool fmi2CoSimulationSlave::readString(const std::vector<unsigned int> &vr, std::vector<const char *> &ref) {
    return AbstractFmuInstance::readString(vr, ref);
}

bool fmi2CoSimulationSlave::readBoolean(unsigned int vr, int &ref) {
    return AbstractFmuInstance::readBoolean(vr, ref);
}

bool fmi2CoSimulationSlave::readBoolean(const std::vector<unsigned int> &vr, std::vector<int> &ref) {
    return AbstractFmuInstance::readBoolean(vr, ref);
}

bool fmi2CoSimulationSlave::writeInteger(unsigned int vr, int value) {
    return AbstractFmuInstance::writeInteger(vr, value);
}

bool fmi2CoSimulationSlave::writeInteger(const std::vector<unsigned int> &vr, const std::vector<int> &values) {
    return AbstractFmuInstance::writeInteger(vr, values);
}

bool fmi2CoSimulationSlave::writeReal(unsigned int vr, double value) {
    return AbstractFmuInstance::writeReal(vr, value);
}

bool fmi2CoSimulationSlave::writeReal(const std::vector<unsigned int> &vr, const std::vector<double> &values) {
    return AbstractFmuInstance::writeReal(vr, values);
}

bool fmi2CoSimulationSlave::writeString(unsigned int vr, const char *value) {
    return AbstractFmuInstance::writeString(vr, value);
}

bool fmi2CoSimulationSlave::writeString(const std::vector<unsigned int> &vr, const std::vector<const char *> &values) {
    return AbstractFmuInstance::writeString(vr, values);
}

bool fmi2CoSimulationSlave::writeBoolean(unsigned int vr, int value) {
    return AbstractFmuInstance::writeBoolean(vr, value);
}

bool fmi2CoSimulationSlave::writeBoolean(const std::vector<unsigned int> &vr, const std::vector<int> &values) {
    return AbstractFmuInstance::writeBoolean(vr, values);
}


bool fmi2CoSimulationSlave::getFMUstate(void *&state) {
    return AbstractFmuInstance::getFMUstate(state);
}

bool fmi2CoSimulationSlave::setFMUstate(void *state) {
    return AbstractFmuInstance::setFMUstate(state);
}

bool fmi2CoSimulationSlave::freeFMUstate(void *&state) {
    return AbstractFmuInstance::freeFMUstate(state);
}

bool fmi2CoSimulationSlave::serializeFMUstate(const fmi2FMUstate &state, std::vector<char> &serializedState) {
    return AbstractFmuInstance::serializeFMUstate(state, serializedState);
}

bool fmi2CoSimulationSlave::deSerializeFMUstate(fmi2FMUstate &state, const std::vector<char> &serializedState) {
    return AbstractFmuInstance::deSerializeFMUstate(state, serializedState);
}

bool fmi2CoSimulationSlave::getDirectionalDerivative(const std::vector<unsigned int> &vUnknownRef,
                                                 const std::vector<unsigned int> &vKnownRef,
                                                 const std::vector<double> &dvKnownRef,
                                                 std::vector<double> &dvUnknownRef) {
    return AbstractFmuInstance::getDirectionalDerivative(vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}



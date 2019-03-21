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

#include <fmi4cpp/fmi2/status_converter.hpp>
#include <fmi4cpp/fmi2/cs_slave.hpp>

using namespace fmi4cpp::fmi2;

cs_slave::cs_slave(const fmi2Component c,
                                     const std::shared_ptr<fmi4cpp::fmu_resource> &resource,
                                     const std::shared_ptr<cs_library> &library,
                                     const std::shared_ptr<const cs_model_description> &modelDescription)
        : fmu_instance_base<cs_library, cs_model_description>(c, resource, library, modelDescription) {}

fmi4cpp::Status cs_slave::last_status() const {
    return convert(library_->getLastStatus());
}

bool cs_slave::step(const double stepSize) {
    if (library_->doStep(c_, simulationTime_, stepSize, false)) {
        simulationTime_ += stepSize;
        return true;
    }
    return false;
}

bool cs_slave::cancel_step() {
    return library_->cancelStep(c_);
}

std::shared_ptr<const cs_model_description> cs_slave::model_description() const {
    return fmu_instance_base::model_description();
}

bool cs_slave::setupExperiment(double start, double stop, double tolerance) {
    return fmu_instance_base::setupExperiment(start, stop, tolerance);
}

bool cs_slave::enterInitializationMode() {
    return fmu_instance_base::enterInitializationMode();
}

bool cs_slave::exitInitializationMode() {
    return fmu_instance_base::exitInitializationMode();
}

bool cs_slave::reset() {
    return fmu_instance_base::reset();
}

bool cs_slave::terminate() {
    return fmu_instance_base::terminate();
}

bool cs_slave::readInteger(unsigned int vr, int &ref) {
    return fmu_instance_base::readInteger(vr, ref);
}

bool cs_slave::readInteger(const std::vector<unsigned int> &vr, std::vector<int> &ref) {
    return fmu_instance_base::readInteger(vr, ref);
}

bool cs_slave::readReal(unsigned int vr, double &ref) {
    return fmu_instance_base::readReal(vr, ref);
}

bool cs_slave::readReal(const std::vector<unsigned int> &vr, std::vector<double> &ref) {
    return fmu_instance_base::readReal(vr, ref);
}

bool cs_slave::readString(unsigned int vr, const char *&ref) {
    return fmu_instance_base::readString(vr, ref);
}

bool cs_slave::readString(const std::vector<unsigned int> &vr, std::vector<const char *> &ref) {
    return fmu_instance_base::readString(vr, ref);
}

bool cs_slave::readBoolean(unsigned int vr, int &ref) {
    return fmu_instance_base::readBoolean(vr, ref);
}

bool cs_slave::readBoolean(const std::vector<unsigned int> &vr, std::vector<int> &ref) {
    return fmu_instance_base::readBoolean(vr, ref);
}

bool cs_slave::writeInteger(unsigned int vr, int value) {
    return fmu_instance_base::writeInteger(vr, value);
}

bool cs_slave::writeInteger(const std::vector<unsigned int> &vr, const std::vector<int> &values) {
    return fmu_instance_base::writeInteger(vr, values);
}

bool cs_slave::writeReal(unsigned int vr, double value) {
    return fmu_instance_base::writeReal(vr, value);
}

bool cs_slave::writeReal(const std::vector<unsigned int> &vr, const std::vector<double> &values) {
    return fmu_instance_base::writeReal(vr, values);
}

bool cs_slave::writeString(unsigned int vr, const char *value) {
    return fmu_instance_base::writeString(vr, value);
}

bool cs_slave::writeString(const std::vector<unsigned int> &vr, const std::vector<const char *> &values) {
    return fmu_instance_base::writeString(vr, values);
}

bool cs_slave::writeBoolean(unsigned int vr, int value) {
    return fmu_instance_base::writeBoolean(vr, value);
}

bool cs_slave::writeBoolean(const std::vector<unsigned int> &vr, const std::vector<int> &values) {
    return fmu_instance_base::writeBoolean(vr, values);
}


bool cs_slave::getFMUstate(void *&state) {
    return fmu_instance_base::getFMUstate(state);
}

bool cs_slave::setFMUstate(void *state) {
    return fmu_instance_base::setFMUstate(state);
}

bool cs_slave::freeFMUstate(void *&state) {
    return fmu_instance_base::freeFMUstate(state);
}

bool cs_slave::serializeFMUstate(const fmi2FMUstate &state, std::vector<char> &serializedState) {
    return fmu_instance_base::serializeFMUstate(state, serializedState);
}

bool cs_slave::deSerializeFMUstate(fmi2FMUstate &state, const std::vector<char> &serializedState) {
    return fmu_instance_base::deSerializeFMUstate(state, serializedState);
}

bool cs_slave::getDirectionalDerivative(const std::vector<unsigned int> &vUnknownRef,
                                                 const std::vector<unsigned int> &vKnownRef,
                                                 const std::vector<double> &dvKnownRef,
                                                 std::vector<double> &dvUnknownRef) {
    return fmu_instance_base::getDirectionalDerivative(vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}



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

#include <fmicpp/fmi2/import/CoSimulationSlave.hpp>

using namespace std;
using namespace fmicpp::fmi2::import;

CoSimulationSlave::CoSimulationSlave(const shared_ptr<CoSimulationLibrary> library)
            : AbstractFmuInstance<CoSimulationLibrary>(library), FmuSlave() {}

fmi2Status CoSimulationSlave::doStep(const double stepSize) {
    return library->doStep(FmuSlave::simulationTime, stepSize, false);
}

fmi2Status CoSimulationSlave::cancelStep() {
    return library->cancelStep();
}

void CoSimulationSlave::init(double start, double stop) {
    AbstractFmuInstance::init(start, stop);
}

fmi2Status CoSimulationSlave::reset() {
    return AbstractFmuInstance::reset();
}

fmi2Status CoSimulationSlave::terminate() {
    return AbstractFmuInstance::terminate();
}

bool CoSimulationSlave::canGetAndSetFMUstate() const {
    return AbstractFmuInstance::canGetAndSetFMUstate();
}

fmi2Status CoSimulationSlave::getFMUstate(fmi2FMUstate &state) {
   return AbstractFmuInstance::getFMUstate(state);
}

fmi2Status CoSimulationSlave::setFMUstate(const fmi2FMUstate state) {
   return AbstractFmuInstance::setFMUstate(state);
}

fmi2Status CoSimulationSlave::freeFMUstate(fmi2FMUstate &state) {
   return AbstractFmuInstance::freeFMUstate(state);
}

bool CoSimulationSlave::canSerializeFmuState() const {
    return AbstractFmuInstance::canSerializeFmuState();
}

fmi2Status CoSimulationSlave::serializeFMUstate(vector<fmi2Byte> &serializedState) {
   return AbstractFmuInstance::terminate();
}

fmi2Status CoSimulationSlave::deSerializeFMUstate(const vector<fmi2Byte> &serializedState, fmi2FMUstate &state) {
   return AbstractFmuInstance::terminate();
}

bool CoSimulationSlave::providesDirectionalDerivative() const {
    return AbstractFmuInstance::providesDirectionalDerivative();
}

fmi2Status CoSimulationSlave::getDirectionalDerivative(const vector<fmi2ValueReference> &vUnkownRef,
                                                       const vector<fmi2ValueReference> &vKnownRef,
                                                       const vector<fmi2Real> &dvKnownRef,
                                                       vector<fmi2Real> &dvUnknownRef) {
   return AbstractFmuInstance::getDirectionalDerivative(vUnkownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}






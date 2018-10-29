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

#include <stdexcept>

#include <cvode/cvode.h>
#include <fmi4cpp/fmi2/import/ModelExchangeSlave.hpp>

using namespace fmi4cpp::fmi2;

namespace {

    const double EPS = 1E-13;

    std::unique_ptr<CoSimulationModelDescription> wrap(const ModelExchangeModelDescription &me) {
        CoSimulationAttributes attributes(me.attributes());
        attributes.canHandleVariableCommunicationStepSize = true;
        attributes.maxOutputDerivativeOrder = 0;
        return std::make_unique<CoSimulationModelDescription>(CoSimulationModelDescription(me, attributes));
    }

}

ModelExchangeSlave::ModelExchangeSlave(
        std::unique_ptr<ModelExchangeInstance> &instance,
        std::unique_ptr<Solver> &solver)
        : instance_(std::move(instance)), solver_(std::move(solver)) {

    csModelDescription_ = std::move(wrap(*instance_->getModelDescription()));

    size_t numberOfContinuousStates = instance_->getModelDescription()->numberOfContinuousStates();
    size_t numberOfEventIndicators = instance_->getModelDescription()->numberOfEventIndicators();

    x_.reserve(numberOfContinuousStates);
    dx_.reserve(numberOfContinuousStates);

    z_.reserve(numberOfEventIndicators);
    pz_.reserve(numberOfEventIndicators);

}

void ModelExchangeSlave::operator()(const std::vector<double> &x, std::vector<double> &dxdt, const double t) {
    instance_->setTime(t);
    instance_->setContinuousStates(x);
    instance_->getDerivatives(dxdt);
}

fmi2Status ModelExchangeSlave::doStep(const double stepSize) {

    if (stepSize <= 0) {
        return fmi2Error;
    }

    double time = simulationTime_;
    double stopTime = (time + stepSize);

    while (time <= stopTime) {

        double tNext = std::min((time + stepSize), stopTime);

        bool timeEvent = eventInfo_.nextEventTimeDefined && eventInfo_.nextEventTime <= time;
        if (timeEvent) {
            tNext = eventInfo_.nextEventTime;
        }

        bool stateEvent = false;
        if ((tNext - time) > EPS) {
            stateEvent = solve(time, tNext);
        }
        time = tNext;

        instance_->setTime(time);

        bool enterEventMode = false;
        if (!instance_->getModelDescription()->completedIntegratorStepNotNeeded()) {
            int enterEventMode_ = 0;
            int terminateSimulation_ = 0;
            instance_->completedIntegratorStep(false, enterEventMode_, terminateSimulation_);
            if (terminateSimulation_ == 1) {
                terminate();
            }
            enterEventMode = (enterEventMode_ == 1);
        }

        if (timeEvent || stateEvent || enterEventMode) {
            instance_->enterEventMode();
        }

        if (eventIteration()) {
            return fmi2Discard;
        }

    }

    simulationTime_ = time;

    return fmi2OK;
}

bool ModelExchangeSlave::solve(double t, double tNext) {

    instance_->getContinuousStates(x_);

    double stepSize = (tNext - t);
    solver_->integrate(*this, t, x_, (simulationTime_ + stepSize));

    pz_ = z_;
    instance_->getEventIndicators(z_);
    for (unsigned int i = 0; i < pz_.size(); i++) {
        if (pz_[i] * z_[i] < 0) {
            return true;
        }
    }

    return false;

}

fmi2Status ModelExchangeSlave::cancelStep() {
    return fmi2Discard;
}

fmi2Status ModelExchangeSlave::reset() {
    return instance_->reset();
}

fmi2Status ModelExchangeSlave::terminate() {
    return instance_->terminate();
}


std::shared_ptr<CoSimulationModelDescription> ModelExchangeSlave::getModelDescription() const {
    return csModelDescription_;
}

bool ModelExchangeSlave::eventIteration() {

    eventInfo_.newDiscreteStatesNeeded = true;
    eventInfo_.terminateSimulation = false;

    while (eventInfo_.newDiscreteStatesNeeded) {
         instance_->newDiscreteStates(eventInfo_);
         if (eventInfo_.terminateSimulation) {
             terminate();
             return true;
         }
    }

    instance_->enterContinuousTimeMode();

    return false;
}

fmi2Status ModelExchangeSlave::setupExperiment(double startTime, double stopTime, double tolerance) {
    return instance_->setupExperiment(startTime, stopTime, tolerance);
}

fmi2Status ModelExchangeSlave::enterInitializationMode() {
    return instance_->enterInitializationMode();
}

fmi2Status ModelExchangeSlave::exitInitializationMode() {
    auto status = instance_->exitInitializationMode();
    if (eventIteration()) {
        throw std::runtime_error("EventIteration failed during initialization!");
    }

    return status;
}


fmi2Status ModelExchangeSlave::getFMUstate(fmi2FMUstate &state) {
    return instance_->getFMUstate(state);
}

fmi2Status ModelExchangeSlave::setFMUstate(fmi2FMUstate state) {
    return instance_->setFMUstate(state);
}

fmi2Status ModelExchangeSlave::freeFMUstate(fmi2FMUstate &state) {
    return instance_->freeFMUstate(state);
}

fmi2Status ModelExchangeSlave::serializeFMUstate(const fmi2FMUstate &state,
                                                 std::vector<fmi2Byte> &serializedState) {
    return instance_->serializeFMUstate(state, serializedState);
}

fmi2Status ModelExchangeSlave::deSerializeFMUstate(fmi2FMUstate &state,
                                                   const std::vector<fmi2Byte> &serializedState) {
    return instance_->deSerializeFMUstate(state, serializedState);
}

fmi2Status
ModelExchangeSlave::getDirectionalDerivative(const std::vector<fmi2ValueReference> &vUnkownRef,
                                             const std::vector<fmi2ValueReference> &vKnownRef,
                                             const std::vector<fmi2Real> &dvKnownRef,
                                             std::vector<fmi2Real> &dvUnknownRef) const {
    return instance_->getDirectionalDerivative(vUnkownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}

fmi2Status ModelExchangeSlave::readInteger(fmi2ValueReference vr, fmi2Integer &ref) const {
    return instance_->readInteger(vr, ref);
}

fmi2Status ModelExchangeSlave::readInteger(const std::vector<fmi2ValueReference> &vr,
                                           std::vector<fmi2Integer> &ref) const {
    return instance_->readInteger(vr, ref);
}

fmi2Status ModelExchangeSlave::readReal(fmi2ValueReference vr, fmi2Real &ref) const {
    return instance_->readReal(vr, ref);
}

fmi2Status ModelExchangeSlave::readReal(const std::vector<fmi2ValueReference> &vr,
                                        std::vector<fmi2Real> &ref) const {
    return instance_->readReal(vr, ref);
}

fmi2Status ModelExchangeSlave::readString(fmi2ValueReference vr, fmi2String &ref) const {
    return instance_->readString(vr, ref);
}

fmi2Status ModelExchangeSlave::readString(const std::vector<fmi2ValueReference> &vr,
                                          std::vector<fmi2String> &ref) const {
    return instance_->readString(vr, ref);
}

fmi2Status ModelExchangeSlave::readBoolean(fmi2ValueReference vr, fmi2Boolean &ref) const {
    return instance_->readBoolean(vr, ref);
}

fmi2Status ModelExchangeSlave::readBoolean(const std::vector<fmi2ValueReference> &vr,
                                           std::vector<fmi2Boolean> &ref) const {
    return instance_->readBoolean(vr, ref);
}

fmi2Status ModelExchangeSlave::writeInteger(fmi2ValueReference vr, fmi2Integer value) {
    return instance_->writeInteger(vr, value);
}

fmi2Status ModelExchangeSlave::writeInteger(const std::vector<fmi2ValueReference> &vr,
                                            const std::vector<fmi2Integer> &values) {
    return instance_->writeInteger(vr, values);
}

fmi2Status ModelExchangeSlave::writeReal(fmi2ValueReference vr, fmi2Real value) {
    return instance_->writeReal(vr, value);
}

fmi2Status ModelExchangeSlave::writeReal(const std::vector<fmi2ValueReference> &vr,
                                         const std::vector<fmi2Real> &values) {
    return instance_->writeReal(vr, values);
}

fmi2Status ModelExchangeSlave::writeString(fmi2ValueReference vr, fmi2String value) {
    return instance_->writeString(vr, value);
}

fmi2Status ModelExchangeSlave::writeString(const std::vector<fmi2ValueReference> &vr,
                                           const std::vector<fmi2String> &values) {
    return instance_->writeString(vr, values);
}

fmi2Status ModelExchangeSlave::writeBoolean(fmi2ValueReference vr, fmi2Boolean value) {
    return instance_->writeBoolean(vr, value);
}

fmi2Status ModelExchangeSlave::writeBoolean(const std::vector<fmi2ValueReference> &vr,
                                            const std::vector<fmi2Boolean> &values) {
    return instance_->writeBoolean(vr, values);
}


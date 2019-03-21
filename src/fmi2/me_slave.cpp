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

#include <utility>

#include <fmi4cpp/logger.hpp>
#include <fmi4cpp/fmi2/me_slave.hpp>

using namespace fmi4cpp::fmi2;
using namespace fmi4cpp::solver;

namespace {

    const double EPS = 1E-13;

    std::shared_ptr<const cs_model_description> wrap(const me_model_description &me) {
        cs_attributes attributes(me);
        attributes.canHandleVariableCommunicationStepSize = true;
        attributes.maxOutputDerivativeOrder = 0;
        return std::make_shared<cs_model_description>(cs_model_description(me, attributes));
    }

}

me_slave::me_slave(
        std::shared_ptr<fmi4cpp::fmu_resource> &resource,
        std::unique_ptr<me_instance> &instance,
        std::unique_ptr<me_solver> &solver)
        : resource_(std::move(resource)), instance_(std::move(instance)), solver_(std::move(solver)) {

    sys_.instance_ = instance_;
    csModelDescription_ = wrap(*instance_->model_description());

    size_t numberOfContinuousStates = instance_->model_description()->numberOfContinuousStates();
    size_t numberOfEventIndicators = instance_->model_description()->numberOfEventIndicators;

    x_ = std::vector<double>(numberOfContinuousStates);

    z_ = std::vector<double>(numberOfEventIndicators);
    pz_ = std::vector<double>(numberOfEventIndicators);

}

std::shared_ptr<const cs_model_description> me_slave::model_description() const {
    return csModelDescription_;
}

bool me_slave::setupExperiment(double startTime, double stopTime, double tolerance) {
    return instance_->setupExperiment(startTime, stopTime, tolerance);
}

bool me_slave::enterInitializationMode() {
    return instance_->enterInitializationMode();
}

bool me_slave::exitInitializationMode() {
    auto status = instance_->exitInitializationMode();

    instance_->eventInfo_.newDiscreteStatesNeeded = fmi2True;
    instance_->eventInfo_.terminateSimulation = fmi2False;

    while (instance_->eventInfo_.newDiscreteStatesNeeded != fmi2False &&
           instance_->eventInfo_.terminateSimulation == fmi2False) {
        instance_->newDiscreteStates();
    }

    instance_->enterContinuousTimeMode();

    return status;
}


bool me_slave::step(const double stepSize) {

    if (stepSize <= 0) {
        fmi4cpp::logger::error("Error: stepSize <= 0");
        return false;
    }

    double time = getSimulationTime();
    double stopTime = (time + stepSize);

    while (time < stopTime) {

        double tNext = std::min((time + stepSize), stopTime);

        bool timeEvent = instance_->eventInfo_.nextEventTimeDefined != fmi2False &&
                (instance_->eventInfo_.nextEventTime <= time);
        if (timeEvent) {
            tNext = instance_->eventInfo_.nextEventTime;
        }

        bool stateEvent = false;
        if ((tNext - time) > EPS) {
            auto result = solve(time, tNext);
            time = result.first;
            stateEvent = result.second;
        } else {
            time = tNext;
        }
        
        instance_->setTime(time);

        bool stepEvent = false;
        if (!instance_->model_description()->completedIntegratorStepNotNeeded) {
            fmi2Boolean enterEventMode_ = fmi2False;
            fmi2Boolean terminateSimulation_ = fmi2False;;
            instance_->completedIntegratorStep(true, enterEventMode_, terminateSimulation_);
            if (terminateSimulation_) {
                terminate();
            }
            stepEvent = enterEventMode_ != fmi2False;
        }

        if (timeEvent || stateEvent || stepEvent) {
            instance_->enterEventMode();

            instance_->eventInfo_.newDiscreteStatesNeeded = fmi2True;
            instance_->eventInfo_.terminateSimulation = fmi2False;

            while (instance_->eventInfo_.newDiscreteStatesNeeded != fmi2False &&
                   instance_->eventInfo_.terminateSimulation == fmi2False) {
                instance_->newDiscreteStates();
            }

            instance_->enterContinuousTimeMode();

        }

    }

    return true;
}

std::pair<double, bool> me_slave::solve(double t, double tNext) {

    instance_->getContinuousStates(x_);

    double integratedTime = solver_->integrate(sys_, x_, t, tNext);

    pz_ = z_;
    instance_->getEventIndicators(z_);
    bool stateEvent = false;
    for (unsigned int i = 0; i < pz_.size(); i++) {
        if ((pz_[i] * z_[i]) < 0) {
            stateEvent = true;
            break;
        }
    }

    return std::make_pair(integratedTime, stateEvent);

}

const double me_slave::getSimulationTime() const {
    return instance_->getSimulationTime();
}

bool me_slave::cancel_step() {
    return false;
}

bool me_slave::reset() {
    return instance_->reset();
}

bool me_slave::terminate() {
    return instance_->terminate();
}


bool me_slave::getFMUstate(fmi2FMUstate &state) {
    return instance_->getFMUstate(state);
}

bool me_slave::setFMUstate(fmi2FMUstate state) {
    return instance_->setFMUstate(state);
}

bool me_slave::freeFMUstate(fmi2FMUstate &state) {
    return instance_->freeFMUstate(state);
}

bool me_slave::serializeFMUstate(const fmi2FMUstate &state,
                                                 std::vector<fmi2Byte> &serializedState) {
    return instance_->serializeFMUstate(state, serializedState);
}

bool me_slave::deSerializeFMUstate(fmi2FMUstate &state,
                                                   const std::vector<fmi2Byte> &serializedState) {
    return instance_->deSerializeFMUstate(state, serializedState);
}

bool me_slave::getDirectionalDerivative(const std::vector<fmi2ValueReference> &vUnknownRef,
                                             const std::vector<fmi2ValueReference> &vKnownRef,
                                             const std::vector<fmi2Real> &dvKnownRef,
                                             std::vector<fmi2Real> &dvUnknownRef) {
    return instance_->getDirectionalDerivative(vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}

bool me_slave::readInteger(fmi2ValueReference vr, fmi2Integer &ref) {
    return instance_->readInteger(vr, ref);
}

bool me_slave::readInteger(const std::vector<fmi2ValueReference> &vr,
                                           std::vector<fmi2Integer> &ref) {
    return instance_->readInteger(vr, ref);
}

bool me_slave::readReal(fmi2ValueReference vr, fmi2Real &ref) {
    return instance_->readReal(vr, ref);
}

bool me_slave::readReal(const std::vector<fmi2ValueReference> &vr,
                                        std::vector<fmi2Real> &ref) {
    return instance_->readReal(vr, ref);
}

bool me_slave::readString(fmi2ValueReference vr, fmi2String &ref) {
    return instance_->readString(vr, ref);
}

bool me_slave::readString(const std::vector<fmi2ValueReference> &vr,
                                          std::vector<fmi2String> &ref) {
    return instance_->readString(vr, ref);
}

bool me_slave::readBoolean(fmi2ValueReference vr, fmi2Boolean &ref) {
    return instance_->readBoolean(vr, ref);
}

bool me_slave::readBoolean(const std::vector<fmi2ValueReference> &vr,
                                           std::vector<fmi2Boolean> &ref) {
    return instance_->readBoolean(vr, ref);
}

bool me_slave::writeInteger(fmi2ValueReference vr, fmi2Integer value) {
    return instance_->writeInteger(vr, value);
}

bool me_slave::writeInteger(const std::vector<fmi2ValueReference> &vr,
                                            const std::vector<fmi2Integer> &values) {
    return instance_->writeInteger(vr, values);
}

bool me_slave::writeReal(fmi2ValueReference vr, fmi2Real value) {
    return instance_->writeReal(vr, value);
}

bool me_slave::writeReal(const std::vector<fmi2ValueReference> &vr,
                                         const std::vector<fmi2Real> &values) {
    return instance_->writeReal(vr, values);
}

bool me_slave::writeString(fmi2ValueReference vr, fmi2String value) {
    return instance_->writeString(vr, value);
}

bool me_slave::writeString(const std::vector<fmi2ValueReference> &vr,
                                           const std::vector<fmi2String> &values) {
    return instance_->writeString(vr, values);
}

bool me_slave::writeBoolean(fmi2ValueReference vr, fmi2Boolean value) {
    return instance_->writeBoolean(vr, value);
}

bool me_slave::writeBoolean(const std::vector<fmi2ValueReference> &vr,
                                            const std::vector<fmi2Boolean> &values) {
    return instance_->writeBoolean(vr, values);
}

fmi4cpp::Status me_slave::last_status() const {
    return instance_->last_status();
}

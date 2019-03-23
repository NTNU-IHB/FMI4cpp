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

#include <fmi4cpp/mlog.hpp>
#include <fmi4cpp/fmi2/me_slave.hpp>

using namespace fmi4cpp::fmi2;
using namespace fmi4cpp::solver;

namespace {

    const double EPS = 1E-13;

    std::shared_ptr<const cs_model_description> wrap(const me_model_description &me) {
        cs_attributes attributes(me);
        attributes.can_handle_variable_communication_step_size = true;
        attributes.max_output_derivative_order = 0;
        return std::make_shared<cs_model_description>(cs_model_description(me, attributes));
    }

}

me_slave::me_slave(
        std::shared_ptr<fmi4cpp::fmu_resource> &resource,
        std::unique_ptr<me_instance> &instance,
        std::unique_ptr<me_solver> &solver)
        : resource_(std::move(resource)), instance_(std::move(instance)), solver_(std::move(solver)) {

    sys_.instance_ = instance_;
    csModelDescription_ = wrap(*instance_->get_model_description());

    size_t numberOfContinuousStates = instance_->get_model_description()->number_of_continuous_states();
    size_t numberOfEventIndicators = instance_->get_model_description()->number_of_event_indicators;

    x_ = std::vector<double>(numberOfContinuousStates);

    z_ = std::vector<double>(numberOfEventIndicators);
    pz_ = std::vector<double>(numberOfEventIndicators);

}

std::shared_ptr<const cs_model_description> me_slave::get_model_description() const {
    return csModelDescription_;
}

bool me_slave::setup_experiment(double startTime, double stopTime, double tolerance) {
    return instance_->setup_experiment(startTime, stopTime, tolerance);
}

bool me_slave::enter_initialization_mode() {
    return instance_->enter_initialization_mode();
}

bool me_slave::exit_initialization_mode() {
    auto status = instance_->exit_initialization_mode();

    instance_->eventInfo_.newDiscreteStatesNeeded = fmi2True;
    instance_->eventInfo_.terminateSimulation = fmi2False;

    while (instance_->eventInfo_.newDiscreteStatesNeeded != fmi2False &&
           instance_->eventInfo_.terminateSimulation == fmi2False) {
        instance_->new_discrete_states();
    }

    instance_->enter_continuous_time_mode();

    return status;
}


bool me_slave::step(const double stepSize) {

    if (stepSize <= 0) {
        MLOG_ERROR("Error: stepSize <= 0");
        return false;
    }

    double time = get_simulation_time();
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

        instance_->set_time(time);

        bool stepEvent = false;
        if (!instance_->get_model_description()->completed_integrator_step_not_needed) {
            fmi2Boolean enterEventMode_ = fmi2False;
            fmi2Boolean terminateSimulation_ = fmi2False;;
            instance_->completed_integrator_step(true, enterEventMode_, terminateSimulation_);
            if (terminateSimulation_) {
                terminate();
            }
            stepEvent = enterEventMode_ != fmi2False;
        }

        if (timeEvent || stateEvent || stepEvent) {
            instance_->enter_event_mode();

            instance_->eventInfo_.newDiscreteStatesNeeded = fmi2True;
            instance_->eventInfo_.terminateSimulation = fmi2False;

            while (instance_->eventInfo_.newDiscreteStatesNeeded != fmi2False &&
                   instance_->eventInfo_.terminateSimulation == fmi2False) {
                instance_->new_discrete_states();
            }

            instance_->enter_continuous_time_mode();

        }

    }

    return true;
}

std::pair<double, bool> me_slave::solve(double t, double tNext) {

    instance_->get_continuous_states(x_);

    double integratedTime = solver_->integrate(sys_, x_, t, tNext);

    pz_ = z_;
    instance_->get_event_indicators(z_);
    bool stateEvent = false;
    for (unsigned int i = 0; i < pz_.size(); i++) {
        if ((pz_[i] * z_[i]) < 0) {
            stateEvent = true;
            break;
        }
    }

    return std::make_pair(integratedTime, stateEvent);

}

const double me_slave::get_simulation_time() const {
    return instance_->get_simulation_time();
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


bool me_slave::get_fmu_state(fmi2FMUstate &state) {
    return instance_->get_fmu_state(state);
}

bool me_slave::set_fmu_state(fmi2FMUstate state) {
    return instance_->set_fmu_state(state);
}

bool me_slave::free_fmu_state(fmi2FMUstate &state) {
    return instance_->free_fmu_state(state);
}

bool me_slave::serialize_fmu_state(const fmi2FMUstate &state,
                                                 std::vector<fmi2Byte> &serializedState) {
    return instance_->serialize_fmu_state(state, serializedState);
}

bool me_slave::de_serialize_fmu_state(fmi2FMUstate &state,
                                                   const std::vector<fmi2Byte> &serializedState) {
    return instance_->de_serialize_fmu_state(state, serializedState);
}

bool me_slave::get_directional_derivative(const std::vector<fmi2ValueReference> &vUnknownRef,
                                             const std::vector<fmi2ValueReference> &vKnownRef,
                                             const std::vector<fmi2Real> &dvKnownRef,
                                             std::vector<fmi2Real> &dvUnknownRef) {
    return instance_->get_directional_derivative(vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}

bool me_slave::read_integer(fmi2ValueReference vr, fmi2Integer &ref) {
    return instance_->read_integer(vr, ref);
}

bool me_slave::read_integer(const std::vector<fmi2ValueReference> &vr,
                                           std::vector<fmi2Integer> &ref) {
    return instance_->read_integer(vr, ref);
}

bool me_slave::read_real(fmi2ValueReference vr, fmi2Real &ref) {
    return instance_->read_real(vr, ref);
}

bool me_slave::read_real(const std::vector<fmi2ValueReference> &vr,
                                        std::vector<fmi2Real> &ref) {
    return instance_->read_real(vr, ref);
}

bool me_slave::read_string(fmi2ValueReference vr, fmi2String &ref) {
    return instance_->read_string(vr, ref);
}

bool me_slave::read_string(const std::vector<fmi2ValueReference> &vr,
                                          std::vector<fmi2String> &ref) {
    return instance_->read_string(vr, ref);
}

bool me_slave::read_boolean(fmi2ValueReference vr, fmi2Boolean &ref) {
    return instance_->read_boolean(vr, ref);
}

bool me_slave::read_boolean(const std::vector<fmi2ValueReference> &vr,
                                           std::vector<fmi2Boolean> &ref) {
    return instance_->read_boolean(vr, ref);
}

bool me_slave::write_integer(fmi2ValueReference vr, fmi2Integer value) {
    return instance_->write_integer(vr, value);
}

bool me_slave::write_integer(const std::vector<fmi2ValueReference> &vr,
                                            const std::vector<fmi2Integer> &values) {
    return instance_->write_integer(vr, values);
}

bool me_slave::write_real(fmi2ValueReference vr, fmi2Real value) {
    return instance_->write_real(vr, value);
}

bool me_slave::write_real(const std::vector<fmi2ValueReference> &vr,
                                         const std::vector<fmi2Real> &values) {
    return instance_->write_real(vr, values);
}

bool me_slave::write_string(fmi2ValueReference vr, fmi2String value) {
    return instance_->write_string(vr, value);
}

bool me_slave::write_string(const std::vector<fmi2ValueReference> &vr,
                                           const std::vector<fmi2String> &values) {
    return instance_->write_string(vr, values);
}

bool me_slave::write_boolean(fmi2ValueReference vr, fmi2Boolean value) {
    return instance_->write_boolean(vr, value);
}

bool me_slave::write_boolean(const std::vector<fmi2ValueReference> &vr,
                                            const std::vector<fmi2Boolean> &values) {
    return instance_->write_boolean(vr, values);
}

fmi4cpp::status me_slave::last_status() const {
    return instance_->last_status();
}

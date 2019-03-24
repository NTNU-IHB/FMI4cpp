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

#ifndef FMI4CPP_FMI2MODELEXCHANGESLAVE_HPP
#define FMI4CPP_FMI2MODELEXCHANGESLAVE_HPP

#include <memory>
#include <vector>

#include <fmi4cpp/fmu_slave.hpp>
#include <fmi4cpp/fmu_resource.hpp>
#include <fmi4cpp/solver/solver.hpp>

#include <fmi4cpp/fmi2/me_instance.hpp>
#include "fmi4cpp/fmi2/xml/cs_model_description.hpp"

namespace fmi4cpp::solver {

    class fmu_wrapper {

    public:

        std::shared_ptr<fmi2::me_instance> instance_;

        void operator()(const state_type &x, state_type &dx, const double t) {
            instance_->set_time(t);
            instance_->set_continuous_states(x);
            instance_->get_derivatives(dx);
        }

    };

}

namespace fmi4cpp::fmi2 {

    class me_slave : public fmu_slave<cs_model_description> {

    private:

        solver::fmu_wrapper sys_;
        std::shared_ptr<fmu_resource> resource_;
        std::shared_ptr<me_instance> instance_;
        std::unique_ptr<fmi4cpp::solver::me_solver> solver_;
        std::shared_ptr<const cs_model_description> csModelDescription_;

        std::vector<fmi2Real> x_;
        std::vector<fmi2Real> z_;
        std::vector<fmi2Real> pz_;

        std::pair<double, bool> solve(double t0, double tNext);

    public:

        me_slave(std::shared_ptr<fmu_resource> &resource, std::unique_ptr<me_instance> &instance,
                           std::unique_ptr<fmi4cpp::solver::me_solver> &solver);

        const double get_simulation_time() const override;
//
        fmi4cpp::status last_status() const override;

        std::shared_ptr<const cs_model_description> get_model_description() const override;

        bool step(double stepSize) override;

        bool cancel_step() override;

        bool setup_experiment(double startTime = 0, double stopTime = 0, double tolerance = 0) override;

        bool enter_initialization_mode() override;

        bool exit_initialization_mode() override;

        bool reset() override;

        bool terminate() override;

        bool read_integer(fmi2ValueReference vr, fmi2Integer &ref) override;

        bool read_integer(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) override;

        bool read_real(fmi2ValueReference vr, fmi2Real &ref) override;

        bool read_real(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) override;

        bool read_string(fmi2ValueReference vr, fmi2String &ref) override;

        bool read_string(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) override;

        bool read_boolean(fmi2ValueReference vr, fmi2Boolean &ref) override;

        bool read_boolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) override;

        bool write_integer(fmi2ValueReference vr, fmi2Integer value) override;

        bool write_integer(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer> &values) override;

        bool write_real(fmi2ValueReference vr, fmi2Real value) override;

        bool write_real(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real> &values) override;

        bool write_string(fmi2ValueReference vr, fmi2String value) override;

        bool write_string(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String> &values) override;

        bool write_boolean(fmi2ValueReference vr, fmi2Boolean value) override;

        bool write_boolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean> &values) override;


        bool get_fmu_state(fmi2FMUstate &state) override;

        bool set_fmu_state(fmi2FMUstate state) override;

        bool free_fmu_state(fmi2FMUstate &state) override;

        bool serialize_fmu_state(const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) override;

        bool de_serialize_fmu_state(fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) override;

        bool get_directional_derivative(const std::vector<fmi2ValueReference> &vUnknownRef,
                                        const std::vector<fmi2ValueReference> &vKnownRef,
                                        const std::vector<fmi2Real> &dvKnownRef,
                                        std::vector<fmi2Real> &dvUnknownRef) override;

    };

}


#endif //FMI4CPP_FMI2MODELEXCHANGESLAVE_HPP

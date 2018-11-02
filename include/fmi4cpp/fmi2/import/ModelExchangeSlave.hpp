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

#ifndef FMI4CPP_MODELEXCHANGESLAVE_HPP
#define FMI4CPP_MODELEXCHANGESLAVE_HPP

#include <memory>
#include <utility>

#include "ModelExchangeInstance.hpp"
#include "FmuSlave.hpp"
#include "Solver.hpp"

namespace fmi4cpp::fmi2 {

    class sys_wrapper {

    public:
        std::shared_ptr<ModelExchangeInstance> instance_;

        void operator() (const std::vector<double> &x, std::vector<double> &dx, const double t) {
            instance_->setTime(t);
            instance_->setContinuousStates(x);
            instance_->getDerivatives(dx);
        }

    };

    class ModelExchangeSlave : public FmuSlave {

    private:

        std::unique_ptr<Solver> solver_;
        std::shared_ptr<ModelExchangeInstance> instance_;
        std::shared_ptr<CoSimulationModelDescription> csModelDescription_;

        std::vector<fmi2Real > x_;

        std::vector<fmi2Real > z_;
        std::vector<fmi2Real > pz_;

        sys_wrapper sys_;

        bool solve(double t0, double tNext);

    public:
        ModelExchangeSlave(std::unique_ptr<ModelExchangeInstance> &instance,
                           std::unique_ptr<Solver> &solver);

        std::shared_ptr<CoSimulationModelDescription> getModelDescription() const override;

        fmi2Status doStep(double stepSize) override;

        fmi2Status cancelStep() override;

        fmi2Status setupExperiment(double startTime = 0, double stopTime = 0, double tolerance = 0) override;

        fmi2Status enterInitializationMode() override;

        fmi2Status exitInitializationMode() override;

        fmi2Status reset() override;

        fmi2Status terminate() override;

        fmi2Status getFMUstate(fmi2FMUstate &state) override;

        fmi2Status setFMUstate(fmi2FMUstate state) override;

        fmi2Status freeFMUstate(fmi2FMUstate &state) override;

        fmi2Status serializeFMUstate(const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) override;

        fmi2Status deSerializeFMUstate(fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) override;

        fmi2Status getDirectionalDerivative(const std::vector<fmi2ValueReference> &vUnknownRef,
                                            const std::vector<fmi2ValueReference> &vKnownRef,
                                            const std::vector<fmi2Real> &dvKnownRef,
                                            std::vector<fmi2Real> &dvUnknownRef) const override;

        fmi2Status readInteger(fmi2ValueReference vr, fmi2Integer &ref) const override;

        fmi2Status readInteger(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) const override;

        fmi2Status readReal(fmi2ValueReference vr, fmi2Real &ref) const override;

        fmi2Status readReal(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) const override;

        fmi2Status readString(fmi2ValueReference vr, fmi2String &ref) const override;

        fmi2Status readString(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) const override;

        fmi2Status readBoolean(fmi2ValueReference vr, fmi2Boolean &ref) const override;

        fmi2Status readBoolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) const override;

        fmi2Status writeInteger(fmi2ValueReference vr, fmi2Integer value) override;

        fmi2Status
        writeInteger(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer> &values) override;

        fmi2Status writeReal(fmi2ValueReference vr, fmi2Real value) override;

        fmi2Status writeReal(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real> &values) override;

        fmi2Status writeString(fmi2ValueReference vr, fmi2String value) override;

        fmi2Status
        writeString(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String> &values) override;

        fmi2Status writeBoolean(fmi2ValueReference vr, fmi2Boolean value) override;

        fmi2Status
        writeBoolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean> &values) override;

        const double getSimulationTime() const override;

    };

}


#endif //FMI4CPP_MODELEXCHANGESLAVE_HPP

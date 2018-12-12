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

#include "fmi2ModelExchangeInstance.hpp"

#include "fmi4cpp/FmuSlave.hpp"
#include "fmi4cpp/solver/ModelExchangeSolver.hpp"
#include "fmi4cpp/fmi2/xml/CoSimulationModelDescription.hpp"


namespace fmi4cpp::solver {

    class fmu_wrapper {

    public:
        std::shared_ptr<fmi2::fmi2ModelExchangeInstance> instance_;

        void operator()(const std::vector<double> &x, std::vector<double> &dx, const double t) {
            instance_->setTime(t);
            instance_->setContinuousStates(x);
            instance_->getDerivatives(dx);
        }

    };

}

namespace fmi4cpp::fmi2 {

    class fmi2ModelExchangeSlave : public FmuSlave<CoSimulationModelDescription> {

    private:

        solver::fmu_wrapper sys_;
        std::unique_ptr<fmi4cpp::solver::ModelExchangeSolver> solver_;
        std::shared_ptr<fmi2ModelExchangeInstance> instance_;
        std::shared_ptr<CoSimulationModelDescription> csModelDescription_;

        std::vector<fmi2Real> x_;
        std::vector<fmi2Real> z_;
        std::vector<fmi2Real> pz_;

        std::pair<double, bool> solve(double t0, double tNext);

    public:

        fmi2ModelExchangeSlave(std::unique_ptr<fmi2ModelExchangeInstance> &instance,
                           std::unique_ptr<fmi4cpp::solver::ModelExchangeSolver> &solver);

        const double getSimulationTime() const override;

        Status getLastStatus() const override;

        std::shared_ptr<CoSimulationModelDescription> getModelDescription() const override;

        bool doStep(double stepSize) override;

        bool cancelStep() override;

        bool setupExperiment(double startTime = 0, double stopTime = 0, double tolerance = 0) override;

        bool enterInitializationMode() override;

        bool exitInitializationMode() override;

        bool reset() override;

        bool terminate() override;

        bool getFMUstate(fmi2FMUstate &state) override;

        bool setFMUstate(fmi2FMUstate state) override;

        bool freeFMUstate(fmi2FMUstate &state) override;

        bool serializeFMUstate(const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) override;

        bool deSerializeFMUstate(fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) override;

        bool getDirectionalDerivative(const std::vector<fmi2ValueReference> &vUnknownRef,
                                            const std::vector<fmi2ValueReference> &vKnownRef,
                                            const std::vector<fmi2Real> &dvKnownRef,
                                            std::vector<fmi2Real> &dvUnknownRef) override;

        bool readInteger(fmi2ValueReference vr, fmi2Integer &ref) override;

        bool readInteger(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) override;

        bool readReal(fmi2ValueReference vr, fmi2Real &ref) override;

        bool readReal(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) override;

        bool readString(fmi2ValueReference vr, fmi2String &ref) override;

        bool readString(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) override;

        bool readBoolean(fmi2ValueReference vr, fmi2Boolean &ref) override;

        bool readBoolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) override;

        bool writeInteger(fmi2ValueReference vr, fmi2Integer value) override;

        bool writeInteger(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer> &values) override;

        bool writeReal(fmi2ValueReference vr, fmi2Real value) override;

        bool writeReal(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real> &values) override;

        bool writeString(fmi2ValueReference vr, fmi2String value) override;

        bool writeString(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String> &values) override;

        bool writeBoolean(fmi2ValueReference vr, fmi2Boolean value) override;

        bool writeBoolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean> &values) override;

    };

}


#endif //FMI4CPP_FMI2MODELEXCHANGESLAVE_HPP

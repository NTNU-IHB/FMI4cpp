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

#ifndef FMI4CPP_FMI2MODELEXCHANGELIBRARY_HPP
#define FMI4CPP_FMI2MODELEXCHANGELIBRARY_HPP

#include <fmi4cpp/fmi2/fmi2_library.hpp>

namespace fmi4cpp::fmi2 {

    class me_library : public fmi2_library {

    private:

        fmi2EnterEventModeTYPE *fmi2EnterEventMode_;
        fmi2EnterContinuousTimeModeTYPE *fmi2EnterContinuousTimeMode_;
        fmi2SetTimeTYPE *fmi2SetTime_;
        fmi2SetContinuousStatesTYPE *fmi2SetContinuousStates_;
        fmi2GetDerivativesTYPE *fmi2GetDerivatives_;
        fmi2GetEventIndicatorsTYPE *fmi2GetEventIndicators_;
        fmi2GetContinuousStatesTYPE *fmi2GetContinuousStates_;
        fmi2GetNominalsOfContinuousStatesTYPE *fmi2GetNominalsOfContinuousStates_;
        fmi2CompletedIntegratorStepTYPE *fmi2CompletedIntegratorStep_;
        fmi2NewDiscreteStatesTYPE *fmi2NewDiscreteStates_;

    public:

        explicit me_library(const std::string &modelIdentifier,
                                          const std::shared_ptr<fmu_resource> &resource);

        bool enter_event_mode(fmi2Component c);

        bool enter_continuous_time_mode(fmi2Component c);

        bool set_time(fmi2Component c, double time);

        bool set_continuous_states(fmi2Component c, const std::vector<fmi2Real> &x);

        bool get_derivatives(fmi2Component c, std::vector<fmi2Real> &derivatives);

        bool get_event_indicators(fmi2Component c, std::vector<fmi2Real> &eventIndicators);

        bool get_continuous_states(fmi2Component c, std::vector<fmi2Real> &x);

        bool get_nominals_of_continuous_states(fmi2Component c, std::vector<fmi2Real> &x_nominal);

        bool completed_integrator_step(fmi2Component c, fmi2Boolean noSetFMUStatePriorToCurrentPoint,
                                       fmi2Boolean &enterEventMode, fmi2Boolean &terminateSimulation);

        bool new_discrete_states(fmi2Component c, fmi2EventInfo &eventInfo);

    };

}

#endif //FMI4CPP_FMI2MODELEXCHANGELIBRARY_HPP

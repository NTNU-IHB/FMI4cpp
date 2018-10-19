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

#ifndef FMI4CPP_MODELEXCHANGELIBRARY_HPP
#define FMI4CPP_MODELEXCHANGELIBRARY_HPP

#include "FmiLibrary.hpp"

namespace fmi4cpp::fmi2 {

    class ModelExchangeLibrary : public FmiLibrary {

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
        explicit ModelExchangeLibrary(const std::string &modelIdentifier, const std::shared_ptr<FmuResource> &resource);

        fmi2Status enterEventMode(fmi2Component c) const;

        fmi2Status enterContinuousTimeMode(fmi2Component c) const;

        fmi2Status setTime(fmi2Component c, double time) const;

        fmi2Status setContinuousStates(fmi2Component c, const std::vector<fmi2Real> &x) const;

        fmi2Status getDerivatives(fmi2Component c, std::vector<fmi2Real> &derivatives) const;

        fmi2Status getEventIndicators(fmi2Component c, std::vector<fmi2Real> &eventIndicators) const;

        fmi2Status getContinuousStates(fmi2Component c, std::vector<fmi2Real> &x) const;

        fmi2Status getNominalsOfContinuousStates(fmi2Component c, std::vector<fmi2Real> &x_nominal) const;

        fmi2Status completedIntegratorStep(fmi2Component c, fmi2Boolean noSetFMUStatePriorToCurrentPoint,
                                           fmi2Boolean &enterEventMode, fmi2Boolean &terminateSimulation) const;

        fmi2Status newDiscreteStates(fmi2Component c, fmi2EventInfo &eventInfo);

    };

}

#endif //FMI4CPP_MODELEXCHANGELIBRARY_HPP

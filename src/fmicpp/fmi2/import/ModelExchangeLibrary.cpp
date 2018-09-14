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

#include <fmicpp/fmi2/import/ModelExchangeLibrary.hpp>


ModelExchangeLibrary::ModelExchangeLibrary(const string &libName) : FmiLibrary(libName) {

    fmi2EnterEventMode_ = loadFunction<fmi2EnterEventModeTYPE *>("fmi2EnterEventMode");
    fmi2EnterContinuousTimeMode_ = loadFunction<fmi2EnterContinuousTimeModeTYPE *>("fmi2EnterContinuousTimeMode");
    fmi2SetTime_ = loadFunction<fmi2SetTimeTYPE *>("fmi2SetTime");
    fmi2SetContinuousStates_ = loadFunction<fmi2SetContinuousStatesTYPE *>("fmi2SetContinuousStates");
    fmi2GetDerivatives_ = loadFunction<fmi2GetDerivativesTYPE *>("fmi2GetDerivatives");
    fmi2GetEventIndicators_ = loadFunction<fmi2GetEventIndicatorsTYPE *>("fmi2GetEventIndicators");
    fmi2GetContinuousStates_ = loadFunction<fmi2GetContinuousStatesTYPE *>("fmi2GetContinuousStates");
    fmi2GetNominalsOfContinuousStates_ = loadFunction<fmi2GetNominalsOfContinuousStatesTYPE *>(
            "fmi2GetNominalsOfContinuousStates");
    fmi2CompletedIntegratorStep_ = loadFunction<fmi2CompletedIntegratorStepTYPE *>("fmi2CompletedIntegratorStep");
    fmi2NewDiscreteStates_ = loadFunction<fmi2NewDiscreteStatesTYPE *>("fmi2NewDiscreteStates");

}

fmi2Status ModelExchangeLibrary::enterEventMode(const fmi2Component c) const {
    return fmi2EnterEventMode_(c);
}

fmi2Status ModelExchangeLibrary::enterContinuousTimeMode(const fmi2Component c) const {
    return fmi2EnterContinuousTimeMode_(c);
}

fmi2Status ModelExchangeLibrary::setTime(const fmi2Component c, const double time) const {
    return fmi2SetTime_(c, time);
}

fmi2Status ModelExchangeLibrary::setContinuousStates(const fmi2Component c, const vector<fmi2Real> &x) const {
    return fmi2SetContinuousStates_(c, x.data(), x.size());
}

fmi2Status ModelExchangeLibrary::getDerivatives(const fmi2Component c, vector<fmi2Real> &derivatives) const {
    return fmi2GetDerivatives_(c, derivatives.data(), derivatives.size());
}

fmi2Status ModelExchangeLibrary::getEventIndicators(const fmi2Component c, vector<fmi2Real> &eventIndicators) const {
    return fmi2GetEventIndicators_(c, eventIndicators.data(), eventIndicators.size());
}

fmi2Status ModelExchangeLibrary::getContinuousStates(const fmi2Component c, vector<fmi2Real> &x) const {
    return fmi2GetContinuousStates_(c, x.data(), x.size());
}

fmi2Status
ModelExchangeLibrary::getNominalsOfContinuousStates(const fmi2Component c, vector<fmi2Real> &x_nominal) const {
    return fmi2GetNominalsOfContinuousStates_(c, x_nominal.data(), x_nominal.size());
}

fmi2Status ModelExchangeLibrary::completedIntegratorStep(const fmi2Component c,
                                                         fmi2Boolean noSetFMUStatePriorToCurrentPoint,
                                                         fmi2Boolean &enterEventMode,
                                                         fmi2Boolean &terminateSimulation) const {
    return fmi2CompletedIntegratorStep_(c, noSetFMUStatePriorToCurrentPoint, &enterEventMode, &terminateSimulation);
}

fmi2Status ModelExchangeLibrary::newDiscreteStates(const fmi2Component c, fmi2EventInfo &eventInfo) {
    return fmi2NewDiscreteStates_(c, &eventInfo);
}

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

#include <fmi4cpp/fmi2/import/fmi2ModelExchangeLibrary.hpp>

#include "../../common/import/FmiLibraryHelper.hpp"

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

fmi2ModelExchangeLibrary::fmi2ModelExchangeLibrary(const std::string &modelIdentifier,
                                                   const std::shared_ptr<FmuResource> &resource)
        : fmi2Library(modelIdentifier, resource) {

    fmi2EnterEventMode_ = loadFunction<fmi2EnterEventModeTYPE *>(handle_, "fmi2EnterEventMode");
    fmi2EnterContinuousTimeMode_ = loadFunction<fmi2EnterContinuousTimeModeTYPE *>(
            handle_, "fmi2EnterContinuousTimeMode");
    fmi2SetTime_ = loadFunction<fmi2SetTimeTYPE *>(handle_, "fmi2SetTime");
    fmi2SetContinuousStates_ = loadFunction<fmi2SetContinuousStatesTYPE *>(handle_, "fmi2SetContinuousStates");
    fmi2GetDerivatives_ = loadFunction<fmi2GetDerivativesTYPE *>(handle_, "fmi2GetDerivatives");
    fmi2GetEventIndicators_ = loadFunction<fmi2GetEventIndicatorsTYPE *>(handle_, "fmi2GetEventIndicators");
    fmi2GetContinuousStates_ = loadFunction<fmi2GetContinuousStatesTYPE *>(handle_, "fmi2GetContinuousStates");
    fmi2GetNominalsOfContinuousStates_ = loadFunction<fmi2GetNominalsOfContinuousStatesTYPE *>(
            handle_, "fmi2GetNominalsOfContinuousStates");
    fmi2CompletedIntegratorStep_ = loadFunction<fmi2CompletedIntegratorStepTYPE *>(handle_,
                                                                                   "fmi2CompletedIntegratorStep");
    fmi2NewDiscreteStates_ = loadFunction<fmi2NewDiscreteStatesTYPE *>(handle_, "fmi2NewDiscreteStates");

}

bool fmi2ModelExchangeLibrary::enterEventMode(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2EnterEventMode_(c));
}

bool fmi2ModelExchangeLibrary::enterContinuousTimeMode(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2EnterContinuousTimeMode_(c));
}

bool fmi2ModelExchangeLibrary::setTime(const fmi2Component c, const double time) {
    return updateStatusAndReturnTrueIfOK(fmi2SetTime_(c, time));
}

bool fmi2ModelExchangeLibrary::setContinuousStates(const fmi2Component c, const std::vector<fmi2Real> &x) {
    return updateStatusAndReturnTrueIfOK(fmi2SetContinuousStates_(c, x.data(), x.size()));
}

bool fmi2ModelExchangeLibrary::getDerivatives(const fmi2Component c, std::vector<fmi2Real> &derivatives) {
    return updateStatusAndReturnTrueIfOK(fmi2GetDerivatives_(c, derivatives.data(), derivatives.size()));
}

bool fmi2ModelExchangeLibrary::getEventIndicators(const fmi2Component c, std::vector<fmi2Real> &eventIndicators) {
    return updateStatusAndReturnTrueIfOK(fmi2GetEventIndicators_(c, eventIndicators.data(), eventIndicators.size()));
}

bool fmi2ModelExchangeLibrary::getContinuousStates(const fmi2Component c, std::vector<fmi2Real> &x) {
    return updateStatusAndReturnTrueIfOK(fmi2GetContinuousStates_(c, x.data(), x.size()));
}

bool fmi2ModelExchangeLibrary::getNominalsOfContinuousStates(const fmi2Component c, std::vector<fmi2Real> &x_nominal) {
    return updateStatusAndReturnTrueIfOK(fmi2GetNominalsOfContinuousStates_(c, x_nominal.data(), x_nominal.size()));
}

bool fmi2ModelExchangeLibrary::completedIntegratorStep(const fmi2Component c,
                                                       const fmi2Boolean noSetFMUStatePriorToCurrentPoint,
                                                       fmi2Boolean &enterEventMode,
                                                       fmi2Boolean &terminateSimulation) {
    return updateStatusAndReturnTrueIfOK(
            fmi2CompletedIntegratorStep_(c, noSetFMUStatePriorToCurrentPoint, &enterEventMode, &terminateSimulation));
}

bool fmi2ModelExchangeLibrary::newDiscreteStates(const fmi2Component c, fmi2EventInfo &eventInfo) {
    return updateStatusAndReturnTrueIfOK(fmi2NewDiscreteStates_(c, &eventInfo));
}

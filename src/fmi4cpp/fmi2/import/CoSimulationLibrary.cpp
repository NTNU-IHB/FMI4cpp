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

#include "FmiLibraryHelper.hpp"
#include <fmi4cpp/fmi2/import/CoSimulationLibrary.hpp>

using namespace fmi4cpp::fmi2::import;

CoSimulationLibrary::CoSimulationLibrary(const std::string &modelIdentifier,
                                         const std::shared_ptr<FmuResource> &resource)
        : FmiLibrary(modelIdentifier, resource) {

    fmi2SetRealInputDerivatives_ = loadFunction<fmi2SetRealInputDerivativesTYPE *>(handle_,
                                                                                   "fmi2SetRealInputDerivatives");
    fmi2GetRealOutputDerivatives_ = loadFunction<fmi2GetRealOutputDerivativesTYPE *>(handle_,
                                                                                     "fmi2GetRealOutputDerivatives");

    fmi2DoStep_ = loadFunction<fmi2DoStepTYPE *>(handle_, "fmi2DoStep");
    fmi2CancelStep_ = loadFunction<fmi2CancelStepTYPE *>(handle_, "fmi2CancelStep");

    fmi2GetStatus_ = loadFunction<fmi2GetStatusTYPE *>(handle_, "fmi2GetStatusTYPE");
    fmi2GetRealStatus_ = loadFunction<fmi2GetRealStatusTYPE *>(handle_, "fmi2GetRealStatusTYPE");
    fmi2GetIntegerStatus_ = loadFunction<fmi2GetIntegerStatusTYPE *>(handle_, "fmi2GetIntegerStatusTYPE");
    fmi2GetBooleanStatus_ = loadFunction<fmi2GetBooleanStatusTYPE *>(handle_, "fmi2GetBooleanStatusTYPE");
    fmi2GetStringStatus_ = loadFunction<fmi2GetStringStatusTYPE *>(handle_, "fmi2GetStringStatusTYPE");

}

fmi2Status CoSimulationLibrary::doStep(const fmi2Component c, const fmi2Real currentCommunicationPoint,
                                       const fmi2Real communicationStepSize,
                                       const bool noSetFMUStatePriorToCurrentPoint) const {
    return fmi2DoStep_(c, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint ? 1 : 0);
}

fmi2Status CoSimulationLibrary::cancelStep(const fmi2Component c) const {
    return fmi2CancelStep_(c);
}

fmi2Status CoSimulationLibrary::setRealInputDerivatives(const fmi2Component c,
                                                        const std::vector<fmi2ValueReference> &vr,
                                                        const std::vector<fmi2Integer> &order,
                                                        const std::vector<fmi2Real> &value) const {
    return fmi2SetRealInputDerivatives_(c, vr.data(), vr.size(), order.data(), value.data());
}

fmi2Status CoSimulationLibrary::getRealOutputDerivatives(const fmi2Component c,
                                                         const std::vector<fmi2ValueReference> &vr,
                                                         const std::vector<fmi2Integer> &order,
                                                         std::vector<fmi2Real> &value) const {
    return fmi2GetRealOutputDerivatives_(c, vr.data(), vr.size(), order.data(), value.data());
}

fmi2Status CoSimulationLibrary::getStatus(const fmi2Component c, const fmi2StatusKind s, fmi2Status &value) const {
    return fmi2GetStatus_(c, s, &value);
}

fmi2Status CoSimulationLibrary::getRealStatus(const fmi2Component c, const fmi2StatusKind s, fmi2Real &value) const {
    return fmi2GetRealStatus_(c, s, &value);
}

fmi2Status
CoSimulationLibrary::getIntegerStatus(const fmi2Component c, const fmi2StatusKind s, fmi2Integer &value) const {
    return fmi2GetIntegerStatus_(c, s, &value);
}

fmi2Status
CoSimulationLibrary::getBooleanStatus(const fmi2Component c, const fmi2StatusKind s, fmi2Boolean &value) const {
    return fmi2GetBooleanStatus_(c, s, &value);
}

fmi2Status
CoSimulationLibrary::getStringStatus(const fmi2Component c, const fmi2StatusKind s, fmi2String &value) const {
    return fmi2GetStringStatus_(c, s, &value);
}

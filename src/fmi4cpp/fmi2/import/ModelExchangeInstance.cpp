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

#include <fmi4cpp/fmi2/import/ModelExchangeInstance.hpp>

using namespace fmi4cpp::fmi2::import;
using fmi4cpp::fmi2::import::ModelExchangeInstance;

ModelExchangeInstance::ModelExchangeInstance(const fmi2Component c,
                                             const std::shared_ptr<ModelExchangeLibrary> &library,
                                             const std::shared_ptr<ModelExchangeModelDescription> &modelDescription)
        : AbstractFmuInstance<ModelExchangeLibrary, ModelExchangeModelDescription>(c, library, modelDescription) {}

fmi2Status ModelExchangeInstance::enterEventMode() const {
    return library_->enterEventMode(c_);
}

fmi2Status ModelExchangeInstance::enterContinuousTimeMode() const {
    return library_->enterContinuousTimeMode(c_);
}

fmi2Status ModelExchangeInstance::setTime(const double time) const {
    return library_->setTime(c_, time);
}

fmi2Status ModelExchangeInstance::setContinuousStates(const std::vector<fmi2Real> &x) const {
    return library_->setContinuousStates(c_, x);
}

fmi2Status ModelExchangeInstance::getDerivatives(std::vector<fmi2Real> &derivatives) const {
    return library_->getDerivatives(c_, derivatives);
}

fmi2Status ModelExchangeInstance::getEventIndicators(std::vector<fmi2Real> &eventIndicators) const {
    return library_->getEventIndicators(c_, eventIndicators);
}

fmi2Status ModelExchangeInstance::getContinuousStates(std::vector<fmi2Real> &x) const {
    return library_->getContinuousStates(c_, x);
}

fmi2Status ModelExchangeInstance::getNominalsOfContinuousStates(std::vector<fmi2Real> &x_nominal) const {
    return library_->getNominalsOfContinuousStates(c_, x_nominal);
}

fmi2Status ModelExchangeInstance::completedIntegratorStep(
        fmi2Boolean noSetFMUStatePriorToCurrentPoint,
        fmi2Boolean &enterEventMode,
        fmi2Boolean &terminateSimulation) const {
    return library_->completedIntegratorStep(c_, noSetFMUStatePriorToCurrentPoint, enterEventMode, terminateSimulation);
}

fmi2Status ModelExchangeInstance::newDiscreteStates(fmi2EventInfo &eventInfo) const {
    return library_->newDiscreteStates(c_, eventInfo);
}

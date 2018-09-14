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

#ifndef FMICPP_MODELEXCHANGEINSTANCE_HPP
#define FMICPP_MODELEXCHANGEINSTANCE_HPP

#include "ModelExchangeLibrary.hpp"
#include "AbstractFmuInstance.hpp"

#include "../xml/ModelDescription.hpp"

namespace fmicpp::fmi2::import {

    using xml::ModelExchangeModelDescription;

    class ModelExchangeInstance : public AbstractFmuInstance<ModelExchangeLibrary, ModelExchangeModelDescription> {

    public:
        ModelExchangeInstance(const fmi2Component c,
                              const shared_ptr<ModelExchangeModelDescription> &modelDescription,
                              const shared_ptr<ModelExchangeLibrary> &library);


        fmi2Status enterEventMode() const;

        fmi2Status enterContinuousTimeMode() const;

        fmi2Status setTime(const double time) const;

        fmi2Status setContinuousStates(const vector<fmi2Real> &x) const;

        fmi2Status getDerivatives(vector<fmi2Real> &derivatives) const;

        fmi2Status getEventIndicators(vector<fmi2Real> &eventIndicators) const;

        fmi2Status getContinuousStates(vector<fmi2Real> &x) const;

        fmi2Status getNominalsOfContinuousStates(vector<fmi2Real> &x_nominal) const;

        fmi2Status completedIntegratorStep(
                fmi2Boolean noSetFMUStatePriorToCurrentPoint,
                fmi2Boolean &enterEventMode, fmi2Boolean &terminateSimulation) const;

        fmi2Status newDiscreteStates(fmi2EventInfo &eventInfo) const;

    };

}

#endif //FMICPP_MODELEXCHANGEINSTANCE_HPP

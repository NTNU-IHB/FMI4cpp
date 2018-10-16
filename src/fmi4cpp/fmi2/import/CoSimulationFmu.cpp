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

#include <fmi4cpp/fmi2/import/Fmu.hpp>
#include <fmi4cpp/fmi2/import/CoSimulationSlave.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

unique_ptr<import::CoSimulationFmu> import::Fmu::asCoSimulationFmu() const {
    shared_ptr<CoSimulationModelDescription> cs = std::move(modelDescription_->asCoSimulationModelDescription());
    return make_unique<CoSimulationFmu>(resource_, cs);
}

unique_ptr<import::ModelExchangeFmu> import::Fmu::asModelExchangeFmu() const {
    shared_ptr<ModelExchangeModelDescription> me = std::move(modelDescription_->asModelExchangeModelDescription());
    return make_unique<ModelExchangeFmu>(resource_, me);
}

import::CoSimulationFmu::CoSimulationFmu(const shared_ptr<import::FmuResource> &resource,
                                         const shared_ptr<xml::CoSimulationModelDescription> &md)
        : resource_(resource), modelDescription_(md) {}

shared_ptr<xml::CoSimulationModelDescription> import::CoSimulationFmu::getModelDescription() const {
    return modelDescription_;
}

unique_ptr<import::FmuSlave> import::CoSimulationFmu::newInstance(const bool visible, const bool loggingOn) {
    shared_ptr<import::CoSimulationLibrary> lib = nullptr;
    string modelIdentifier = modelDescription_->modelIdentifier();
    if (modelDescription_->canBeInstantiatedOnlyOncePerProcess()) {
        lib = make_shared<CoSimulationLibrary>(modelIdentifier, resource_);
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<CoSimulationLibrary>(modelIdentifier, resource_);
        }
        lib = lib_;
    }
    fmi2Component c = lib->instantiate(modelIdentifier, fmi2CoSimulation, guid(),
                                       resource_->getResourcePath(), visible, loggingOn);
    return make_unique<import::CoSimulationSlave>(c, lib, modelDescription_);
}

import::CoSimulationFmu::~CoSimulationFmu() {
//#if FMI4CPP_DEBUG_LOGGING_ENABLED
//    cout << "~CoSimulationFmu()" << endl;
//#endif
}

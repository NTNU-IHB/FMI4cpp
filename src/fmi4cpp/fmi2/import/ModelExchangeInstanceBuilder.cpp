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

#include <fmi4cpp/fmi2/import/ModelExchangeInstanceBuilder.hpp>

using std::make_unique;
using std::make_shared;
using namespace fmi4cpp::fmi2::import;

ModelExchangeInstanceBuilder::ModelExchangeInstanceBuilder(Fmu &fmu) : InstanceBuilder(fmu) {}

unique_ptr<ModelExchangeInstance> ModelExchangeInstanceBuilder::newInstance(const bool visible, const bool loggingOn) {
    shared_ptr<ModelExchangeModelDescription> modelDescription
            = fmu_.getModelDescription().asModelExchangeModelDescription();
    shared_ptr<ModelExchangeLibrary> lib = nullptr;
    string modelIdentifier = modelDescription->modelIdentifier();
    if (modelDescription->canBeInstantiatedOnlyOncePerProcess()) {
        lib = make_shared<ModelExchangeLibrary>(fmu_.getAbsoluteLibraryPath(modelIdentifier));
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<ModelExchangeLibrary>(fmu_.getAbsoluteLibraryPath(modelIdentifier));
        }
        lib = lib_;
    }
    fmi2Component c = lib->instantiate(modelIdentifier, fmi2ModelExchange, modelDescription->guid(),
                                       fmu_.getResourcePath(), visible, loggingOn);
    return make_unique<ModelExchangeInstance>(c, modelDescription, lib);
}

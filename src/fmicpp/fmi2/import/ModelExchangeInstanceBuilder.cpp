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

#include <fmicpp/fmi2/import/ModelExchangeInstanceBuilder.hpp>

using namespace fmicpp::fmi2::import;

ModelExchangeInstanceBuilder::ModelExchangeInstanceBuilder(Fmu &fmu): fmu_(fmu) {}

unique_ptr<ModelExchangeInstance> ModelExchangeInstanceBuilder::newInstance(const bool visible, const bool loggingOn) const {
    shared_ptr<ModelExchangeModelDescription> modelDescription = fmu_.getModelDescription().asModelExchangeFmu();
    shared_ptr<ModelExchangeLibrary> lib(new ModelExchangeLibrary(fmu_.getAbsoluteLibraryPath(modelDescription->modelIdentifier)));
    fmi2Component c = lib->instantiate(modelDescription->modelIdentifier,
                                       fmi2ModelExchange, modelDescription->guid, fmu_.getResourcePath(), visible, loggingOn);
    return std::make_unique<ModelExchangeInstance>(c, modelDescription, lib);
}

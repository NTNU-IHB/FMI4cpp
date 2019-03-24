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

#include <fmi4cpp/fmi2/cs_fmu.hpp>

using namespace std;

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

cs_fmu::cs_fmu(const shared_ptr<fmu_resource> &resource,
               const shared_ptr<const cs_model_description> &md)
        : resource_(resource), modelDescription_(md) {}

shared_ptr<const cs_model_description> cs_fmu::get_model_description() const {
    return modelDescription_;
}

unique_ptr<cs_slave> cs_fmu::new_instance(const bool loggingOn, const bool visible) {
    shared_ptr<cs_library> lib = nullptr;
    auto modelIdentifier = modelDescription_->model_identifier;
    if (modelDescription_->can_be_instantiated_only_once_per_process) {
        lib = make_shared<cs_library>(modelIdentifier, resource_);
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<cs_library>(modelIdentifier, resource_);
        }
        lib = lib_;
    }
    auto c = lib->instantiate(modelIdentifier, fmi2CoSimulation, guid(),
                              resource_->resource_path(), loggingOn, visible);
    return make_unique<cs_slave>(c, resource_, lib, modelDescription_);
}


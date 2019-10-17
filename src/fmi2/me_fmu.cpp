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

#include <fmi4cpp/fmi2/me_fmu.hpp>
#include <fmi4cpp/fmi2/me_instance.hpp>

#include <utility>

using namespace std;

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

me_fmu::me_fmu(shared_ptr<fmu_resource> resource,
    shared_ptr<const me_model_description> md)
    : resource_(std::move(resource))
    , modelDescription_(std::move(md))
{}


shared_ptr<const me_model_description> me_fmu::get_model_description() const
{
    return modelDescription_;
}

std::unique_ptr<me_instance> me_fmu::new_instance(const bool visible, const bool loggingOn)
{
    shared_ptr<me_library> lib = nullptr;
    auto modelIdentifier = modelDescription_->model_identifier;
    if (lib_ == nullptr) {
        lib_ = make_shared<me_library>(modelIdentifier, resource_);
    }
    lib = lib_;

    fmi2Component c = lib->instantiate(modelIdentifier, fmi2ModelExchange, guid(),
        resource_->resource_path(), visible, loggingOn);
    return make_unique<me_instance>(c, resource_, lib, modelDescription_);
}

std::unique_ptr<me_slave>
me_fmu::new_instance(std::unique_ptr<me_solver>& solver, const bool visible, const bool loggingOn)
{
    unique_ptr<me_instance> instance = new_instance(visible, loggingOn);
    return make_unique<me_slave>(resource_, instance, solver);
}

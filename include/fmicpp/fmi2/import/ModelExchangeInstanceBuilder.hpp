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

#ifndef FMICPP_MODELEXCHANGEINSTANCEBUILDER_HPP
#define FMICPP_MODELEXCHANGEINSTANCEBUILDER_HPP

#include "InstanceBuilder.hpp"
#include "ModelExchangeInstance.hpp"

namespace fmicpp::fmi2::import {

    class ModelExchangeInstanceBuilder : private InstanceBuilder<ModelExchangeLibrary, ModelExchangeInstance> {

    public:
        explicit ModelExchangeInstanceBuilder(Fmu &fmu);

        std::unique_ptr<ModelExchangeInstance>
        newInstance(const bool visible = false, const bool loggingOn = false) override;

    };

}

#endif //FMICPP_MODELEXCHANGEINSTANCEBUILDER_HPP

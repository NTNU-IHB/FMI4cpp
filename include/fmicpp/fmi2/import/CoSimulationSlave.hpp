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

#ifndef FMICPP_COSIMULATIONSLAVE_HPP
#define FMICPP_COSIMULATIONSLAVE_HPP

#include <memory>
#include "FmuSlave.hpp"
#include "AbstractFmuInstance.hpp"
#include "CoSimulationLibrary.hpp"

#include "../xml/ModelDescription.hpp"

namespace fmicpp::fmi2::import {

    using xml::CoSimulationModelDescription;

    class CoSimulationSlave: public FmuSlave,
            public AbstractFmuInstance<CoSimulationLibrary, CoSimulationModelDescription>  {

    public:
        explicit CoSimulationSlave(const fmi2Component c,
                const std::shared_ptr<CoSimulationModelDescription> modelDescription,
                const std::shared_ptr<CoSimulationLibrary> library);

        fmi2Status doStep(const double stepSize) override;

        fmi2Status cancelStep() override;

    };

}


#endif //FMICPP_COSIMULATIONSLAVE_HPP

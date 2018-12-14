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

#ifndef FMI4CPP_FMI2MODELEXCHANGEFMU_H
#define FMI4CPP_FMI2MODELEXCHANGEFMU_H

#include "fmi4cpp/common/import/Fmu.hpp"
#include "fmi4cpp/common/import/FmuResource.hpp"

#include "fmi4cpp/fmi2/import/fmi2ModelExchangeSlave.hpp"
#include "fmi4cpp/fmi2/xml/ModelExchangeModelDescription.hpp"

namespace fmi4cpp::fmi2 {

    class fmi2ModelExchangeFmu : public virtual ModelExchangeFmu<fmi2ModelExchangeInstance, fmi2ModelExchangeSlave, ModelExchangeModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<fmi2ModelExchangeLibrary> lib_;
        std::shared_ptr<ModelExchangeModelDescription> modelDescription_;

    public:

        fmi2ModelExchangeFmu(const std::shared_ptr<FmuResource> &resource,
                             const std::shared_ptr<ModelExchangeModelDescription> &md);

        std::shared_ptr<ModelExchangeModelDescription> getModelDescription() const override;

        std::unique_ptr<fmi2ModelExchangeInstance> newInstance(bool visible = false, bool loggingOn = false);

        std::unique_ptr<fmi2ModelExchangeSlave>
        newInstance(std::unique_ptr<fmi4cpp::solver::ModelExchangeSolver> &solver, bool visible = false, bool loggingOn = false);

    };

}

#endif //FMI4CPP_FMI2MODELEXCHANGEFMU_H

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

#ifndef FMI4CPP_FMI2COSIMULATIONFMU_H
#define FMI4CPP_FMI2COSIMULATIONFMU_H

#include "fmi4cpp/common/import/Fmu.hpp"
#include "fmi4cpp/common/import/FmuResource.hpp"

#include "fmi4cpp/fmi2/xml/CoSimulationModelDescription.hpp"
#include "fmi4cpp/fmi2/import/fmi2CoSimulationLibrary.hpp"
#include <fmi4cpp/fmi2/import/fmi2CoSimulationSlave.hpp>

namespace fmi4cpp::fmi2 {
    
    class fmi2CoSimulationFmu : public CoSimulationFmu<fmi2CoSimulationSlave, CoSimulationModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<fmi2CoSimulationLibrary> lib_;
        std::shared_ptr<CoSimulationModelDescription> modelDescription_;

    public:

        fmi2CoSimulationFmu(const std::shared_ptr<FmuResource> &resource,
                            const std::shared_ptr<CoSimulationModelDescription> &md);

        std::shared_ptr<CoSimulationModelDescription> getModelDescription() const override;

        std::unique_ptr<fmi2CoSimulationSlave> newInstance(bool visible = false, bool loggingOn = false);

    };

}

#endif //FMI4CPP_FMI2COSIMULATIONFMU_H

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

#ifndef FMI4CPP_FMI2FMU_HPP
#define FMI4CPP_FMI2FMU_HPP

#include <memory>
#include <string>

#include "fmi2CoSimulationFmu.hpp"
#include "fmi2ModelExchangeFmu.hpp"

#include "fmi4cpp/common/import/Fmu.hpp"

#include "fmi4cpp/fmi2/xml/CoSimulationModelDescription.hpp"
#include "fmi4cpp/fmi2/xml/ModelExchangeModelDescription.hpp"

namespace fmi4cpp::fmi2 {

class fmi2Fmu : public virtual FmuProvider<ModelDescription, fmi2CoSimulationFmu, fmi2ModelExchangeFmu> {

        friend class fmi2CoSimulationFmu;
        friend class fmi2ModelExchangeFmu;

    private:

        const std::string fmuName_;
        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<ModelDescription> modelDescription_;

    public:
        explicit fmi2Fmu(const std::string &fmuFile);

        const std::string getFmuName() const;

        const std::string getModelDescriptionXml() const;

        std::shared_ptr<ModelDescription> getModelDescription() const override;

        bool supportsModelExchange() const override;

        bool supportsCoSimulation() const override;

        std::unique_ptr<fmi2CoSimulationFmu> asCoSimulationFmu() const override;

        std::unique_ptr<fmi2ModelExchangeFmu> asModelExchangeFmu() const override;

    };

}


#endif //FMI4CPP_FMI2FMU_HPP

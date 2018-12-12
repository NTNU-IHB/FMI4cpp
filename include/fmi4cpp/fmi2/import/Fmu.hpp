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

#ifndef FMI4CPP_FMU_HPP
#define FMI4CPP_FMU_HPP

#include <memory>
#include <vector>
#include <string>
#include <type_traits>

#include "FmuResource.hpp"
#include "CoSimulationLibrary.hpp"
#include "ModelExchangeLibrary.hpp"
#include "ModelExchangeInstance.hpp"
#include "ModelExchangeSlave.hpp"

#include "FmuSlave.hpp"
#include "../xml/ModelDescription.hpp"

namespace fmi4cpp::fmi2 {

    class CoSimulationFmu;

    class ModelExchangeFmu;

    template<class T>
    class FmuBase {

        static_assert(std::is_base_of<ModelDescriptionBase, T>::value, "T must derive from ModelDescription");

    public:
        std::string guid() const {
            return getModelDescription()->guid();
        }

        std::string modelName() const {
            return getModelDescription()->modelName();
        }

        virtual std::shared_ptr<T> getModelDescription() const = 0;

    };

    class FmuProvider : public virtual FmuBase<ModelDescription> {

    public:
        virtual bool supportsCoSimulation() const = 0;

        virtual bool supportsModelExchange() const = 0;

        virtual std::unique_ptr<CoSimulationFmu> asCoSimulationFmu() const = 0;

        virtual std::unique_ptr<ModelExchangeFmu> asModelExchangeFmu() const = 0;

    };


    class Fmu : public virtual FmuProvider {

        friend class CoSimulationFmu;

        friend class ModelExchangeFmu;

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<ModelDescription> modelDescription_;

    public:
        explicit Fmu(const std::string &fmuFile);

        const std::string fmuFile_;
        
        const std::string getFmuFileName() const;

        const std::string getModelDescriptionXml() const;

        std::shared_ptr<ModelDescription> getModelDescription() const override;

        bool supportsModelExchange() const override;

        bool supportsCoSimulation() const override;

        std::unique_ptr<CoSimulationFmu> asCoSimulationFmu() const override;

        std::unique_ptr<ModelExchangeFmu> asModelExchangeFmu() const override;

    };

    class CoSimulationFmu : public FmuBase<CoSimulationModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<CoSimulationLibrary> lib_;
        std::shared_ptr<CoSimulationModelDescription> modelDescription_;

    public:

        CoSimulationFmu(const std::shared_ptr<FmuResource> &resource,
                        const std::shared_ptr<CoSimulationModelDescription> &md);

        std::shared_ptr<CoSimulationModelDescription> getModelDescription() const override;

        std::unique_ptr<FmuSlave> newInstance(bool visible = false, bool loggingOn = false);

    };

    class ModelExchangeFmu : public virtual FmuBase<ModelExchangeModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<ModelExchangeLibrary> lib_;
        std::shared_ptr<ModelExchangeModelDescription> modelDescription_;

    public:

        ModelExchangeFmu(const std::shared_ptr<FmuResource> &resource,
                         const std::shared_ptr<ModelExchangeModelDescription> &md);

        std::shared_ptr<ModelExchangeModelDescription> getModelDescription() const override;

        std::unique_ptr<ModelExchangeInstance> newInstance(bool visible = false, bool loggingOn = false);

        std::unique_ptr<ModelExchangeSlave>
        newInstance(std::unique_ptr<solver::ModelExchangeSolver> &solver, bool visible = false, bool loggingOn = false);

    };

}


#endif //FMI4CPP_FMU_HPP

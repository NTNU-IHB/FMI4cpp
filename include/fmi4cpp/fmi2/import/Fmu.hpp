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

#include "FmuSlave.hpp"
#include "FmuResource.hpp"
#include "CoSimulationLibrary.hpp"
#include "ModelExchangeLibrary.hpp"
#include "ModelExchangeInstance.hpp"

#include "../xml/ModelDescription.hpp"

using fmi4cpp::fmi2::xml::ModelDescription;

namespace fmi4cpp::fmi2::import {

    class CoSimulationFmu;

    class ModelExchangeFmu;

    template<class T>
    class IFmu {

        static_assert(std::is_base_of<xml::ModelDescriptionBase, T>::value, "T must derive from ModelDescription");

    public:
        std::string guid() const {
            return getModelDescription()->guid();
        }

        std::string modelName() const {
            return getModelDescription()->modelName();
        }

        virtual std::shared_ptr<T> getModelDescription() const = 0;

    };

    class FmuProvider : public virtual IFmu<xml::ModelDescription> {

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

        const std::string getModelDescriptionXml() const;

        std::shared_ptr<ModelDescription> getModelDescription() const override;

        bool supportsModelExchange() const override;

        bool supportsCoSimulation() const override;

        std::unique_ptr<CoSimulationFmu> asCoSimulationFmu() const override;

        std::unique_ptr<ModelExchangeFmu> asModelExchangeFmu() const override;

        ~Fmu();
        
    };

    class CoSimulationFmu : public virtual IFmu<xml::CoSimulationModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<CoSimulationLibrary> lib_;
        std::shared_ptr<xml::CoSimulationModelDescription> modelDescription_;

    public:

        explicit CoSimulationFmu(const std::shared_ptr<FmuResource> &resource,
                                 const std::shared_ptr<xml::CoSimulationModelDescription> &md);

        std::shared_ptr<xml::CoSimulationModelDescription> getModelDescription() const override;

        std::unique_ptr<import::FmuSlave> newInstance(bool visible = false, bool loggingOn = false);

        virtual ~CoSimulationFmu();

    };

    class ModelExchangeFmu : public virtual IFmu<xml::ModelExchangeModelDescription> {

    private:

        std::shared_ptr<FmuResource> resource_;
        std::shared_ptr<ModelExchangeLibrary> lib_;
        std::shared_ptr<xml::ModelExchangeModelDescription> modelDescription_;

    public:

        explicit ModelExchangeFmu(const std::shared_ptr<FmuResource> &resource,
                                  const std::shared_ptr<xml::ModelExchangeModelDescription> &md);

        std::shared_ptr<xml::ModelExchangeModelDescription> getModelDescription() const override;

        std::unique_ptr<import::ModelExchangeInstance> newInstance(bool visible = false, bool loggingOn = false);

        virtual ~ModelExchangeFmu();

    };

}


#endif //FMI4CPP_FMU_HPP

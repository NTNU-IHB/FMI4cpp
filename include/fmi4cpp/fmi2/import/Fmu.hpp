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
#include <experimental/filesystem>

#include "FmuSlave.hpp"
#include "ModelExchangeInstance.hpp"
#include "CoSimulationLibrary.hpp"
#include "ModelExchangeLibrary.hpp"
#include "../xml/ModelDescriptionImpl.hpp"


using fmi4cpp::fmi2::xml::ModelDescriptionProvider;

namespace fs = std::experimental::filesystem;

namespace fmi4cpp::fmi2::import {

    class CoSimulationFmu;

    class ModelExchangeFmu;

    template<class T>
    class IFmu {

        static_assert(std::is_base_of<xml::ModelDescription, T>::value, "T must derive from ModelDescription");

    public:

        std::string getGuid() const {
            return getModelDescription().getGuid();
        }

        std::string getModelName() const {
            return getModelDescription().getModelName();
        }

        virtual const T &getModelDescription() const = 0;

    };

    class FmuProvider : public virtual IFmu<xml::ModelDescriptionProvider> {

    public:
        virtual bool supportsCoSimulation() const = 0;

        virtual bool supportsModelExchange() const = 0;

        virtual CoSimulationFmu &asCoSimulationFmu() = 0;

        virtual ModelExchangeFmu &asModelExchangeFmu() = 0;

    };


    class Fmu : public virtual FmuProvider {

        friend class CoSimulationFmu;

        friend class ModelExchangeFmu;

    private:

        fs::path tmpPath_;
        const std::string fmuFile_;
        std::shared_ptr<ModelDescriptionProvider> modelDescription_;

        std::unique_ptr<CoSimulationFmu> csFmu;
        std::unique_ptr<ModelExchangeFmu> meFmu;

        std::string getResourcePath() const;

        std::string getModelDescriptionPath() const;

        std::string getAbsoluteLibraryPath(std::string modelIdentifier) const;


    public:
        explicit Fmu(const std::string &fmuFile);

        std::string getModelDescriptionXml() const;

        const ModelDescriptionProvider &getModelDescription() const override;

        bool supportsModelExchange() const override;

        bool supportsCoSimulation() const override;

        CoSimulationFmu &asCoSimulationFmu() override;

        ModelExchangeFmu &asModelExchangeFmu() override;

        ~Fmu();

    };

    class CoSimulationFmu : public virtual IFmu<xml::CoSimulationModelDescription> {

    private:
        Fmu &fmu_;
        std::shared_ptr<CoSimulationLibrary> lib_;
        const xml::CoSimulationModelDescription modelDescription_;

    public:
        explicit CoSimulationFmu(Fmu &fmu) : fmu_(fmu), modelDescription_(
                fmu.getModelDescription().asCoSimulationModelDescription()) {};

        const xml::CoSimulationModelDescription &getModelDescription() const override;

        std::unique_ptr<import::FmuSlave> newInstance(bool visible = false, bool loggingOn = false);

    };

    class ModelExchangeFmu: public virtual IFmu<xml::ModelExchangeModelDescription> {

    private:
        Fmu &fmu_;
        std::shared_ptr<ModelExchangeLibrary> lib_;
        const xml::ModelExchangeModelDescription modelDescription_;

    public:
        explicit ModelExchangeFmu(Fmu &fmu): fmu_(fmu), modelDescription_(
                fmu.getModelDescription().asModelExchangeModelDescription()) {};

        const xml::ModelExchangeModelDescription &getModelDescription() const override;

        std::unique_ptr<import::ModelExchangeInstance> newInstance(bool visible = false, bool loggingOn = false);

    };

}


#endif //FMI4CPP_FMU_HPP

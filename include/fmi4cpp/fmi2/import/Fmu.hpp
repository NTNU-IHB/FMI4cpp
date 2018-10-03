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
#include <experimental/filesystem>
#include "../xml/ModelDescriptionImpl.hpp"

using fmi4cpp::fmi2::xml::ModelDescriptionProvider;

namespace fs = std::experimental::filesystem;

namespace fmi4cpp::fmi2::import {

    class CoSimulationSlaveBuilder;

    class ModelExchangeInstanceBuilder;

    class Fmu {

        friend class CoSimulationSlaveBuilder;

        friend class ModelExchangeInstanceBuilder;

    private:

        fs::path tmp_path_;
        const std::string fmu_file_;
        std::shared_ptr<ModelDescriptionProvider> modelDescription_;

        std::string getResourcePath() const;

        std::string getModelDescriptionPath() const;

        std::string getAbsoluteLibraryPath(const string modelIdentifier) const;


    public:
        explicit Fmu(const std::string &fmu_file);

        std::string getGuid() const;

        std::string getModelName() const;

        std::string getModelDescriptionXml() const;

        ModelDescriptionProvider &getModelDescription();

        bool supportsModelExchange() const;

        bool supportsCoSimulation() const;

        CoSimulationSlaveBuilder asCoSimulationFmu();

        ModelExchangeInstanceBuilder asModelExchangeFmu();

        ~Fmu();

    };

}


#endif //FMI4CPP_FMU_HPP

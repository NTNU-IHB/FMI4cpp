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
#include "../xml/ModelDescription.hpp"

using std::string;
using std::unique_ptr;
using fmi4cpp::fmi2::xml::ModelDescription;

namespace fs = std::experimental::filesystem;

namespace fmi4cpp::fmi2::import {

    class CoSimulationSlaveBuilder;

    class ModelExchangeInstanceBuilder;

    class Fmu {

        friend class CoSimulationSlaveBuilder;

        friend class ModelExchangeInstanceBuilder;

    public:
        explicit Fmu(const string &fmu_file);

        const string getGuid() const;

        const string getModelName() const;

        const string getModelDescriptionXml() const;

        const ModelDescription &getModelDescription() const;

        const bool supportsModelExchange() const;

        const bool supportsCoSimulation() const;

        CoSimulationSlaveBuilder &asCoSimulationFmu();

        ModelExchangeInstanceBuilder &asModelExchangeFmu();

        ~Fmu();

    private:

        fs::path tmp_path_;
        const string uuid_;
        const string fmu_file_;
        unique_ptr<ModelDescription> modelDescription_;
        unique_ptr<CoSimulationSlaveBuilder> csBuilder_;
        unique_ptr<ModelExchangeInstanceBuilder> meBuilder_;

        const string getResourcePath() const;

        const string getModelDescriptionPath() const;

        const string getAbsoluteLibraryPath(const string modelIdentifier) const;

    };

}


#endif //FMI4CPP_FMU_HPP

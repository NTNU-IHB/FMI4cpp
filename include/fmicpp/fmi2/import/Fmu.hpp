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

#ifndef FMICPP_FMU_HPP
#define FMICPP_FMU_HPP

#include <memory>
#include <vector>
#include <boost/filesystem.hpp>

#include "FmuInstance.hpp"
#include "CoSimulationSlave.hpp"

#include "../xml/ModelDescription.hpp"

namespace fs = boost::filesystem;
using fmicpp::fmi2::xml::ModelDescription;

namespace fmicpp::fmi2::import {

    class CoSimulationSlaveBuilder;

    class Fmu {

        friend class CoSimulationSlaveBuilder;

    public:
        explicit Fmu(const std::string fmu_file);

        const ModelDescription &getModelDescription() const;

        const std::string &getModelDescriptionXml() const;

        unique_ptr<CoSimulationSlaveBuilder> asCoSimulationFmu();

        ~Fmu();

    private:

        fs::path tmp_path_;
        const std::string fmu_file_;
        std::string model_description_xml_;
        std::unique_ptr<ModelDescription> modelDescription_;
        std::vector<std::shared_ptr<FmuInstance>> instances_;

        std::string getAbsoluteLibraryPath(std::string modelIdentifier);

    };

}


#endif //FMICPP_FMU_HPP

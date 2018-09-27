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

#ifndef FMI4CPP_MODELDESCRIPTION_HPP
#define FMI4CPP_MODELDESCRIPTION_HPP

#include <memory>
#include <string>
#include <optional>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "ModelStructure.hpp"
#include "ModelVariables.hpp"
#include "DefaultExperiment.hpp"
#include "FmuTypeAttributes.hpp"

using std::string;
using std::unique_ptr;
using std::shared_ptr;
using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    class CoSimulationModelDescription;

    class ModelExchangeModelDescription;

    class ModelDescription {

    public:

        virtual string getGuid() const = 0;

        virtual string getFmiVersion() const = 0;

        virtual string getModelName() const = 0;

        virtual string getDescription() const = 0;

        virtual string getVersion() const = 0;

        virtual string getAuthor() const = 0;

        virtual string getLicense() const = 0;

        virtual string getCopyright() const = 0;

        virtual string getGenerationTool() const = 0;

        virtual string getGenerationDateAndTime() const = 0;

        virtual string getVariableNamingConvention() const = 0;

        virtual unsigned int getNumberOfEventIndicators() const = 0;

        virtual unsigned int getNumberOfContinuousStates() const = 0;

        virtual ModelVariables &getModelVariables() = 0;

        virtual const ModelStructure &getModelStructure() const = 0;

        virtual const std::optional<DefaultExperiment> getDefaultExperiment() const = 0;

        virtual bool supportsModelExchange() const = 0;

        virtual bool supportsCoSimulation() const = 0;

        virtual ScalarVariable &getVariableByName(const string &name) = 0;

        virtual ScalarVariable &getVariableByValueReference(const fmi2ValueReference vr) = 0;

        virtual CoSimulationModelDescription asCoSimulationModelDescription() = 0;

        virtual ModelExchangeModelDescription asModelExchangeModelDescription() = 0;

    };

}

#endif //FMI4CPP_MODELDESCRIPTION_HPP

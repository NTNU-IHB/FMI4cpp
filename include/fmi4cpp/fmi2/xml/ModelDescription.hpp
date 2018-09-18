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

    private:
        string guid_;
        string fmiVersion_;
        string modelName_;
        string description_;
        string version_;
        string author_;
        string license_;
        string copyright_;
        string generationTool_;
        string generationDateAndTime_;
        string variableNamingConvention_;

        unsigned int numberOfEventIndicators_;

        ModelVariables modelVariables_;
        ModelStructure modelStructure_;
        std::optional<DefaultExperiment> defaultExperiment_;

        shared_ptr<CoSimulationAttributes> coSimulation_ = nullptr;
        shared_ptr<ModelExchangeAttributes> modelExchange_ = nullptr;

    public:

        string guid() const;

        string fmiVersion() const;

        string modelName() const;

        string description() const;

        string version() const;

        string author() const;

        string license() const;

        string copyright() const;

        string generationTool() const;

        string generationDateAndTime() const;

        string variableNamingConvention() const;

        unsigned int numberOfEventIndicators() const;

        unsigned int numberOfContinuousStates() const;

        const ModelVariables &modelVariables() const;

        const ModelStructure &modelStructure() const;

        const std::optional<DefaultExperiment> defaultExperiment() const;

        bool supportsModelExchange() const;

        bool supportsCoSimulation() const;

        ScalarVariable getVariableByName(const string &name) const;

        ScalarVariable getVariableByValueReference(const fmi2ValueReference vr) const;

        shared_ptr<CoSimulationModelDescription> asCoSimulationModelDescription() const;

        shared_ptr<ModelExchangeModelDescription> asModelExchangeModelDescription() const;

        void load(const string &fileName);

    };

}

#endif //FMI4CPP_MODELDESCRIPTION_HPP

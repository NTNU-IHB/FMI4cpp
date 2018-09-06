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

#ifndef FMICPP_MODELDESCRIPTION_HPP
#define FMICPP_MODELDESCRIPTION_HPP

#include <memory>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "DefaultExperiment.hpp"

#include "ModelVariables.hpp"
#include "FmuTypeAttributes.hpp"

using std::string;
using std::unique_ptr;
using std::shared_ptr;
using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {

    struct CoSimulationModelDescription;
    struct ModelExchangeModelDescription;

    struct ModelDescription {

        string guid;
        string fmiVersion;
        string modelName;
        string description;
        string version;
        string author;
        string license;
        string copyright;
        string generationTool;
        string generationDateAndTime;

        unsigned int numberOfEventIndicators;

        shared_ptr<DefaultExperiment> defaultExperiment = nullptr;
        shared_ptr<ModelVariables> modelVariables = nullptr;

        shared_ptr<CoSimulationModelDescription> asCoSimulationFmu() const;
        shared_ptr<ModelExchangeModelDescription> asModelExchangeFmu() const;

        ScalarVariable &getVariableByName(const string &name) const;
        ScalarVariable &getVariableByValueReference(const fmi2ValueReference vr) const;

        bool supportsModelExchange;
        bool supportsCoSimulation;

        void load(const string fileName);

    private:
        shared_ptr<CoSimulationAttributes> coSimulation = nullptr;
        shared_ptr<ModelExchangeAttributes> modelExchange = nullptr;

    };

    struct SpecificModelDescription: ModelDescription {

        const string modelIdentifier;

        const bool canGetAndSetFMUstate;
        const bool canSerializeFMUstate;
        const bool needsExecutionTool;
        const bool canNotUseMemoryManagementFunctions;
        const bool canBeInstantiatedOnlyOncePerProcess;
        const bool providesDirectionalDerivative;

        const std::shared_ptr<SourceFiles> sourceFiles = nullptr;

        explicit SpecificModelDescription(const ModelDescription modelDescription, const FmuTypeAttributes data);

    };

    struct CoSimulationModelDescription : SpecificModelDescription {

        const bool canInterpolateInputs;
        const bool canRunAsynchronuously;
        const bool canHandleVariableCommunicationStepSize;

        const unsigned int maxOutputDerivativeOrder;

        explicit CoSimulationModelDescription(const ModelDescription modelDescription, const CoSimulationAttributes data);

    };

    struct ModelExchangeModelDescription : SpecificModelDescription {

        const unsigned int numberOfEventIndicators;
        const bool completedIntegratorStepNotNeeded;

        explicit ModelExchangeModelDescription(const ModelDescription modelDescription, const ModelExchangeAttributes data);

    };

}

#endif //FMICPP_MODELDESCRIPTION_HPP

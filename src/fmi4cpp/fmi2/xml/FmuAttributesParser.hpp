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

#ifndef FMI4CPP_FMUATTRIBUTESPARSER_HPP
#define FMI4CPP_FMUATTRIBUTESPARSER_HPP

#include <fmi4cpp/fmi2/xml/FmuAttributes.hpp>

namespace fmi4cpp::fmi2 {

    FmuAttributes parseFmuAttributes(const ptree &node) {

        auto modelIdentifier = node.get<std::string>("<xmlattr>.modelIdentifier");

        auto needsExecutionTool = node.get<bool>("xmlattr>.needsExecutionTool", false);
        auto canGetAndSetFMUstate = node.get<bool>("xmlattr>.canGetAndSetFMUstate", false);
        auto canSerializeFMUstate = node.get<bool>("xmlattr>.canSerializeFMUstate", false);
        auto providesDirectionalDerivative = node.get<bool>("xmlattr>.providesDirectionalDerivative", false);
        auto canNotUseMemoryManagementFunctions = node.get<bool>("xmlattr>.canNotUseMemoryManagementFunctions", false);
        auto canBeInstantiatedOnlyOncePerProcess = node.get<bool>("xmlattr>.canBeInstantiatedOnlyOncePerProcess",
                                                                  false);

        SourceFiles sourceFiles;
        for (const ptree::value_type &v : node) {
            if (v.first == "SourceFiles") {
                parseSourceFiles(v.second, sourceFiles);
            }
        }

        return FmuAttributes(modelIdentifier, canGetAndSetFMUstate, canSerializeFMUstate, needsExecutionTool,
                             canNotUseMemoryManagementFunctions, canBeInstantiatedOnlyOncePerProcess,
                             providesDirectionalDerivative, sourceFiles);

    }

    CoSimulationAttributes parseCoSimulationAttributes(const ptree &node) {

        auto commonAttributes = parseFmuAttributes(node);
        auto maxOutputDerivativeOrder = node.get<size_t>("<xmlattr>.maxOutputDerivativeOrder", 0);
        auto canInterpolateInputs = node.get<bool>("<xmlattr>.canInterpolateInputs", false);
        auto canRunAsynchronuously = node.get<bool>("<xmlattr>.canRunAsynchronuously", false);
        auto canHandleVariableCommunicationStepSize = node.get<bool>("<xmlattr>.canHandleVariableCommunicationStepSize",
                                                                     false);

        return CoSimulationAttributes(commonAttributes, canInterpolateInputs, canRunAsynchronuously,
                                      canHandleVariableCommunicationStepSize, maxOutputDerivativeOrder);

    }

    ModelExchangeAttributes parseModelExchangeAttributes(const ptree &node) {
        auto commonAttributes = parseFmuAttributes(node);
        auto completedIntegratorStepNotNeeded = node.get<bool>("<xmlattr>.completedIntegratorStepNotNeeded", false);
        return ModelExchangeAttributes(commonAttributes, completedIntegratorStepNotNeeded);
    }

}

#endif //FMI4CPP_FMUATTRIBUTESPARSER_HPP

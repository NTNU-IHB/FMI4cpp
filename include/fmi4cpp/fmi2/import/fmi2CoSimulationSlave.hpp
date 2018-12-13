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

#ifndef FMI4CPP_FMI2COSIMULATIONSLAVE_HPP
#define FMI4CPP_FMI2COSIMULATIONSLAVE_HPP

#include <memory>

#include "fmi2CoSimulationLibrary.hpp"

#include "fmi4cpp/common/import/FmuSlave.hpp"
#include "fmi4cpp/common/import/AbstractFmuInstance.hpp"

#include "fmi4cpp/fmi2/xml/ModelDescription.hpp"

#include "../fmi2TypesPlatform.h"

namespace fmi4cpp::fmi2 {

    class fmi2CoSimulationSlave : public virtual FmuSlave<CoSimulationModelDescription>,
                              public AbstractFmuInstance<fmi2CoSimulationLibrary, CoSimulationModelDescription> {

    public:

        fmi2CoSimulationSlave(fmi2Component c,
                          const std::shared_ptr<fmi2CoSimulationLibrary> &library,
                          const std::shared_ptr<CoSimulationModelDescription> &modelDescription);

        bool doStep(double stepSize) override;

        bool cancelStep() override;


        virtual std::shared_ptr<CoSimulationModelDescription> getModelDescription() const;

        virtual Status getLastStatus() const;


        virtual bool setupExperiment(double start = 0, double stop = 0, double tolerance = 0);

        virtual bool enterInitializationMode();

        virtual bool exitInitializationMode();

        virtual bool reset();

        virtual bool terminate();


        virtual bool readInteger(fmi2ValueReference vr, fmi2Integer &ref);

        virtual bool readInteger(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref);

        virtual bool readReal(fmi2ValueReference vr, fmi2Real &ref);

        virtual bool readReal(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref);

        virtual bool readString(fmi2ValueReference vr, fmi2String &ref);

        virtual bool readString(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref);

        virtual bool readBoolean(fmi2ValueReference vr, fmi2Boolean &ref);

        virtual bool readBoolean(const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean > &ref);

        virtual bool writeInteger(fmi2ValueReference vr, fmi2Integer value);

        virtual bool writeInteger(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Integer > &values);

        virtual bool writeReal(fmi2ValueReference vr, fmi2Real value);

        virtual bool writeReal(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real > &values);

        virtual bool writeString(fmi2ValueReference vr, fmi2String value);

        virtual bool writeString(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2String> &values);

        virtual bool writeBoolean(fmi2ValueReference vr, fmi2Boolean value);

        virtual bool writeBoolean(const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Boolean > &values);

        virtual bool getFMUstate(fmi2FMUstate &state);

        virtual bool setFMUstate(fmi2FMUstate state);

        virtual bool freeFMUstate(fmi2FMUstate &state);

        virtual bool serializeFMUstate(const fmi2FMUstate &state, std::vector<fmi2Byte > &serializedState);

        virtual bool deSerializeFMUstate(fmi2FMUstate &state, const std::vector<fmi2Byte > &serializedState);

        virtual bool getDirectionalDerivative(const std::vector<fmi2ValueReference> &vUnknownRef,
                                              const std::vector<fmi2ValueReference> &vKnownRef,
                                              const std::vector<fmi2Real > &dvKnownRef, std::vector<fmi2Real> &dvUnknownRef);

    };

}


#endif //FMI4CPP_FMI2COSIMULATIONSLAVE_HPP

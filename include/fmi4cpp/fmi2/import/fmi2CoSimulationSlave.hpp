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

#include "fmi2Slave.hpp"
#include "fmi2CoSimulationLibrary.hpp"

#include "fmi4cpp/AbstractFmuInstance.hpp"
#include "fmi4cpp/fmi2/xml/ModelDescription.hpp"

namespace fmi4cpp::fmi2 {

    class fmi2CoSimulationSlave : public virtual fmi2Slave,
                              public AbstractFmuInstance<fmi2CoSimulationLibrary, CoSimulationModelDescription> {

    public:
        fmi2CoSimulationSlave(fmi2Component c,
                          const std::shared_ptr<fmi2CoSimulationLibrary> &library,
                          const std::shared_ptr<CoSimulationModelDescription> &modelDescription);

        bool doStep(double stepSize) override;

        bool cancelStep() override;

        virtual std::shared_ptr<CoSimulationModelDescription> getModelDescription() const;

        virtual Status getLastStatus() const;

        virtual bool setupExperiment(double start, double stop, double tolerance);

        virtual bool enterInitializationMode();

        virtual bool exitInitializationMode();

        virtual bool reset();

        virtual bool terminate();


        virtual bool readInteger(unsigned int vr, int &ref);

        virtual bool readInteger(const std::vector<unsigned int> &vr, std::vector<int> &ref);

        virtual bool readReal(unsigned int vr, double &ref);

        virtual bool readReal(const std::vector<unsigned int> &vr, std::vector<double> &ref);

        virtual bool readString(unsigned int vr, const char *&ref);

        virtual bool readString(const std::vector<unsigned int> &vr, std::vector<const char *> &ref);

        virtual bool readBoolean(unsigned int vr, int &ref);

        virtual bool readBoolean(const std::vector<unsigned int> &vr, std::vector<int> &ref);

        virtual bool writeInteger(unsigned int vr, int value);

        virtual bool writeInteger(const std::vector<unsigned int> &vr, const std::vector<int> &values);

        virtual bool writeReal(unsigned int vr, double value);

        virtual bool writeReal(const std::vector<unsigned int> &vr, const std::vector<double> &values);

        virtual bool writeString(unsigned int vr, const char *value);

        virtual bool writeString(const std::vector<unsigned int> &vr, const std::vector<const char *> &values);

        virtual bool writeBoolean(unsigned int vr, int value);

        virtual bool writeBoolean(const std::vector<unsigned int> &vr, const std::vector<int> &values);

        virtual bool getFMUstate(void *&state);

        virtual bool setFMUstate(void *state);

        virtual bool freeFMUstate(void *&state);

        virtual bool serializeFMUstate(const fmi2FMUstate &state, std::vector<char> &serializedState);

        virtual bool deSerializeFMUstate(fmi2FMUstate &state, const std::vector<char> &serializedState);

        virtual bool getDirectionalDerivative(const std::vector<unsigned int> &vUnknownRef,
                                              const std::vector<unsigned int> &vKnownRef,
                                              const std::vector<double> &dvKnownRef, std::vector<double> &dvUnknownRef);

    };

}


#endif //FMI4CPP_FMI2COSIMULATIONSLAVE_HPP

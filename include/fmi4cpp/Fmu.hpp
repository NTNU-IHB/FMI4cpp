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
#include <string>

#include "fmi4cpp/solver/ModelExchangeSolver.hpp"

namespace fmi4cpp {

    template<class ModelDescription>
    class FmuBase {

    public:
        std::string guid() const {
            return getModelDescription()->guid();
        }

        std::string modelName() const {
            return getModelDescription()->modelName();
        }

        virtual std::shared_ptr <ModelDescription> getModelDescription() const = 0;

    };

    template<class ModelDescription, class CoSimulationFmu, class ModelExchangeFmu>
    class FmuProvider : public virtual FmuBase<ModelDescription> {

    public:
        virtual bool supportsCoSimulation() const = 0;

        virtual bool supportsModelExchange() const = 0;

        virtual std::unique_ptr <CoSimulationFmu> asCoSimulationFmu() const = 0;

        virtual std::unique_ptr <ModelExchangeFmu> asModelExchangeFmu() const = 0;

    };

    template <class CoSimulationSlave, class CoSimulationModelDescription>
    class CoSimulationFmu : public FmuBase<CoSimulationModelDescription> {

    public:

        virtual std::shared_ptr<CoSimulationModelDescription> getModelDescription() const = 0;

        virtual std::unique_ptr<CoSimulationSlave> newInstance(bool visible, bool loggingOn) = 0;

    };

    template <class ModelExchangeInstance, class ModelExchangeSlave, class ModelExchangeModelDescription>
    class ModelExchangeFmu : public FmuBase<ModelExchangeModelDescription> {

    public:

        virtual std::shared_ptr<ModelExchangeModelDescription> getModelDescription() const override = 0;

        virtual std::unique_ptr<ModelExchangeInstance> newInstance(bool visible = false, bool loggingOn = false) = 0;

        virtual std::unique_ptr<ModelExchangeSlave>
        newInstance(std::unique_ptr<fmi4cpp::solver::ModelExchangeSolver> &solver, bool visible = false, bool loggingOn = false) = 0;

    };

}


#endif //FMI4CPP_FMU_HPP

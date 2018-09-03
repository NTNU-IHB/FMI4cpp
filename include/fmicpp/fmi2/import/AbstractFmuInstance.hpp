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

#ifndef FMICPP_ABSTRACTFMUINSTANCE_HPP
#define FMICPP_ABSTRACTFMUINSTANCE_HPP

#include <memory>
#include <type_traits>
#include "FmuInstance.hpp"
#include "FmiLibrary.hpp"
#include "../xml/ModelDescription.hpp"

namespace fmicpp::fmi2::import {

    template<typename T, typename U>
    class AbstractFmuInstance: virtual public FmuInstance {

        static_assert(std::is_base_of<FmiLibrary, T>::value, "T must derive from FmiLibrary");
        static_assert(std::is_base_of<xml::SpecificModelDescription, U>::value, "U must derive from SpecificModelDescription");

    protected:

        fmi2Component c_;
        std::shared_ptr<T> library_;
        const std::shared_ptr<U> modelDescription_;

    public:

        explicit AbstractFmuInstance(const std::shared_ptr<U> modelDescription, const std::shared_ptr<T> library);

        void init(const double start = 0, const double stop = 0) override;

        const U &getModelDescription() const override;

        fmi2Status reset() override;

        fmi2Status terminate() override;

        bool canGetAndSetFMUstate() const override;

        fmi2Status getFMUstate(fmi2FMUstate &state) override;

        fmi2Status setFMUstate(const fmi2FMUstate state) override;

        fmi2Status freeFMUstate(fmi2FMUstate &state) override;

        bool canSerializeFmuState() const override;

        fmi2Status serializeFMUstate(vector<fmi2Byte> &serializedState) override;

        fmi2Status deSerializeFMUstate(const vector<fmi2Byte> &serializedState, fmi2FMUstate &state) override;

        bool providesDirectionalDerivative() const override;

        fmi2Status getDirectionalDerivative(const vector<fmi2ValueReference> &vUnkownRef,
                                            const vector<fmi2ValueReference> &vKnownRef,
                                            const vector<fmi2Real> &dvKnownRef,
                                            vector<fmi2Real> &dvUnknownRef) const override;

        ~AbstractFmuInstance();

    };


}

#endif //FMICPP_ABSTRACTFMUINSTANCE_HPP

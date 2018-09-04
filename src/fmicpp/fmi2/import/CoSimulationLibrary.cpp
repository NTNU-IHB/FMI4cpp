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

#include <fmicpp/fmi2/import/CoSimulationLibrary.hpp>

using namespace fmicpp::fmi2::import;

CoSimulationLibrary::CoSimulationLibrary(const string libName) : FmiLibrary(libName) {}

fmi2Status CoSimulationLibrary::doStep(const fmi2Component c, const fmi2Real currentCommunicationPoint,
                                       const fmi2Real communicationStepSize, const bool noSetFMUStatePriorToCurrentPoint) const {
    return loadFunction<fmi2DoStepTYPE *>("fmi2DoStep")(
            c, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint ? 1 : 0);
}

fmi2Status CoSimulationLibrary::cancelStep(const fmi2Component c) const {
    return loadFunction<fmi2CancelStepTYPE *>("fmi2CancelStep")(c);
}


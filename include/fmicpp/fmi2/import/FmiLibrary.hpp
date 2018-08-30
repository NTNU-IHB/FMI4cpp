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

#ifndef FMICPP_FMILIBRARY_HPP
#define FMICPP_FMILIBRARY_HPP

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "fmi2FunctionTypes.h"

#if defined(_MSC_VER) || defined(WIN32) || defined(__MINGW32__)
#include <windows.h>
#define DLL_HANDLE HANDLE
#else
#define DLL_HANDLE void*
#include <dlfcn.h>
#endif

#ifdef WIN32
#define function_ptr FARPROC
#else
typedef void *function_ptr;
#endif

#include <string>
#include <vector>
#include "fmi2Functions.h"

using std::string;
using std::vector;

namespace fmicpp::fmi2::import {

    class FmiLibrary {

    public:

        explicit FmiLibrary(const string lib_name);

        fmi2String getVersion() const;

        fmi2String getTypesPlatform() const;

        fmi2Status enterInitializationMode(fmi2Component c) const;

        fmi2Status exitInitializationMode(fmi2Component c) const;

        fmi2Status setupExperiment(fmi2Component c,
                bool toleranceDefined, double tolerance, double startTime, double stopTime) const;

        fmi2Component instantiate(const string instanceName, fmi2Type type,
                const string guid, const string resourceLocation, bool visible = false, bool loggingOn = false);

        fmi2Status reset(fmi2Component c) const;

        fmi2Status terminate(fmi2Component c) const;

        fmi2Status getInteger(fmi2Component c, const vector<fmi2ValueReference> &vr, vector<fmi2Integer > values) const;

        fmi2Status getReal(fmi2Component c, const vector<fmi2ValueReference> &vr, vector<fmi2Real > values) const;

        void freeInstance(fmi2Component c) const;

        ~FmiLibrary();


    protected:

        DLL_HANDLE handle_ = nullptr;

        template <class T>
        T loadFunction(const char* function_name) const;


    };

    class CoSimulationLibrary : public FmiLibrary {

    public:

        explicit CoSimulationLibrary(const string &lib_name);

        fmi2Status doStep(fmi2Component c,
                fmi2Real currentCommunicationPoint, fmi2Real communicationStepSize, bool noSetFMUStatePriorToCurrentPoint) const;

        fmi2Status cancelStep(fmi2Component c) const;

    };

}


#endif //FMICPP_FMILIBRARY_HPP

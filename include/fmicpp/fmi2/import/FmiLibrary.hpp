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
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "../fmi2FunctionTypes.h"

#if defined(_MSC_VER) || defined(WIN32) || defined(__MINGW32__)
#include <windows.h>
#define DLL_HANDLE HMODULE
#else
#define DLL_HANDLE void*
#include <dlfcn.h>
#endif

#ifdef WIN32
#define function_ptr FARPROC
#else
typedef void *function_ptr;
#endif

using std::string;
using std::vector;

namespace fmicpp::fmi2::import {

    class FmiLibrary {

    private:
        DLL_HANDLE handle_ = nullptr;
        string getLastError() const;

        fmi2GetVersionTYPE * fmi2GetVersion_;
        fmi2GetTypesPlatformTYPE * fmi2GetTypesPlatform_;

        fmi2InstantiateTYPE * fmi2Instantiate_;

        fmi2GetIntegerTYPE * fmi2GetInteger_;
        fmi2GetRealTYPE * fmi2GetReal_;
        fmi2GetStringTYPE * fmi2GetString_;
        fmi2GetBooleanTYPE * fmi2GetBoolean_;

        fmi2SetIntegerTYPE * fmi2SetInteger_;
        fmi2SetRealTYPE * fmi2SetReal_;
        fmi2SetStringTYPE * fmi2SetString_;
        fmi2SetBooleanTYPE * fmi2SetBoolean_;

        fmi2FreeInstanceTYPE * fmi2FreeInstance_;

    protected:

        template<class T>
        T loadFunction(const char *function_name) const {
#ifdef WIN32
            return (T) GetProcAddress(handle_, function_name);
#else
            return (T) dlsym(handle_, function_name);
#endif
        }

    public:

        explicit FmiLibrary(const string &libName);

        fmi2String getVersion() const;

        fmi2String getTypesPlatform() const;

        fmi2Component instantiate(const string instanceName, const fmi2Type type,
                                  const string guid, const string resourceLocation,
                                  const bool visible = false, const bool loggingOn = false);

        fmi2Status setupExperiment(const fmi2Component c, const bool toleranceDefined,
                                   const double tolerance, const double startTime, const double stopTime) const;

        fmi2Status enterInitializationMode(const fmi2Component c) const;
        fmi2Status exitInitializationMode(const fmi2Component c) const;

        fmi2Status reset(const fmi2Component c) const;
        fmi2Status terminate(const fmi2Component c);

        fmi2Status readInteger(const fmi2Component c, const fmi2ValueReference vr, fmi2Integer &ref) const;
        fmi2Status readInteger(const fmi2Component c, const vector<fmi2ValueReference> &vr, vector<fmi2Integer > &ref) const;

        fmi2Status readReal(const fmi2Component c, const fmi2ValueReference vr, fmi2Real &ref) const;
        fmi2Status readReal(const fmi2Component c, const vector<fmi2ValueReference> &vr, vector<fmi2Real > &ref) const;

        fmi2Status readString(const fmi2Component c, const fmi2ValueReference vr, fmi2String &ref) const;
        fmi2Status readString(const fmi2Component c, const vector<fmi2ValueReference> &vr, vector<fmi2String > &ref) const;

        fmi2Status readBoolean(const fmi2Component c, const fmi2ValueReference vr, fmi2Boolean &ref) const;
        fmi2Status readBoolean(const fmi2Component c, const vector<fmi2ValueReference> &vr, vector<fmi2Boolean > &ref) const;

        fmi2Status writeInteger(const fmi2Component c, const fmi2ValueReference vr, fmi2Integer &value) const;
        fmi2Status writeInteger(const fmi2Component c, const vector<fmi2ValueReference> &vr, const vector<fmi2Integer > &values) const;

        fmi2Status writeReal(const fmi2Component c, const fmi2ValueReference vr, fmi2Real &value) const;
        fmi2Status writeReal(const fmi2Component c, const vector<fmi2ValueReference> &vr, const vector<fmi2Real > &values) const;

        fmi2Status writeString(const fmi2Component c, const fmi2ValueReference vr, fmi2String &value) const;
        fmi2Status writeString(const fmi2Component c, const vector<fmi2ValueReference> &vr, const vector<fmi2String > &values) const;

        fmi2Status writeBoolean(const fmi2Component c, const fmi2ValueReference vr, const fmi2Boolean &value) const;
        fmi2Status writeBoolean(const fmi2Component c, const vector<fmi2ValueReference> &vr, const vector<fmi2Boolean > &values) const;

        fmi2Status getFMUstate(const fmi2Component c, fmi2FMUstate &state) const;
        fmi2Status setFMUstate(const fmi2Component c, const fmi2FMUstate state) const;
        fmi2Status freeFMUstate(const fmi2Component c, fmi2FMUstate &state) const;

        fmi2Status serializeFMUstate(const fmi2Component c, const fmi2FMUstate &state, vector<fmi2Byte> &serializedState) const;
        fmi2Status deSerializeFMUstate(const fmi2Component c, fmi2FMUstate &state, const vector<fmi2Byte> &serializedState) const;

        fmi2Status getDirectionalDerivative(const fmi2Component c,
                const vector<fmi2ValueReference> &vUnkownRef, const vector<fmi2ValueReference> &vKnownRef,
                const vector<fmi2Real> &dvKnownRef, vector<fmi2Real> &dvUnknownRef) const;

        void freeInstance(fmi2Component c);

        ~FmiLibrary();

    };

}

#endif //FMICPP_FMILIBRARY_HPP

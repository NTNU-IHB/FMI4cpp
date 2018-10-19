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

#ifndef FMI4CPP_FMILIBRARY_HPP
#define FMI4CPP_FMILIBRARY_HPP

#include <string>
#include <vector>

#include <cstdio>
#include <cstdlib>

#include "../fmi2FunctionTypes.h"

#if defined(_MSC_VER) || defined(WIN32) || defined(__MINGW32__)
#define WIN32_LEAN_AND_MEAN
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

#include "FmuResource.hpp"

namespace fmi4cpp::fmi2 {

    class FmiLibrary {

    private:

        std::shared_ptr<FmuResource> resource_;
        
        fmi2GetVersionTYPE *fmi2GetVersion_;
        fmi2GetTypesPlatformTYPE *fmi2GetTypesPlatform_;

        fmi2SetDebugLoggingTYPE *fmi2SetDebugLogging_;

        fmi2InstantiateTYPE *fmi2Instantiate_;
        fmi2SetupExperimentTYPE *fmi2SetupExperiment_;
        fmi2EnterInitializationModeTYPE *fmi2EnterInitializationMode_;
        fmi2ExitInitializationModeTYPE *fmi2ExitInitializationMode_;

        fmi2ResetTYPE *fmi2Reset_;
        fmi2TerminateTYPE *fmi2Terminate_;

        fmi2GetIntegerTYPE *fmi2GetInteger_;
        fmi2GetRealTYPE *fmi2GetReal_;
        fmi2GetStringTYPE *fmi2GetString_;
        fmi2GetBooleanTYPE *fmi2GetBoolean_;

        fmi2SetIntegerTYPE *fmi2SetInteger_;
        fmi2SetRealTYPE *fmi2SetReal_;
        fmi2SetStringTYPE *fmi2SetString_;
        fmi2SetBooleanTYPE *fmi2SetBoolean_;

        fmi2GetFMUstateTYPE *fmi2GetFMUstate_;
        fmi2SetFMUstateTYPE *fmi2SetFMUstate_;
        fmi2FreeFMUstateTYPE *fmi2FreeFMUstate_;

        fmi2SerializedFMUstateSizeTYPE *fmi2SerializedFMUstateSize_;
        fmi2SerializeFMUstateTYPE *fmi2SerializeFMUstate_;
        fmi2DeSerializeFMUstateTYPE *fmi2DeSerializeFMUstate_;

        fmi2GetDirectionalDerivativeTYPE *fmi2GetDirectionalDerivative_;

        fmi2FreeInstanceTYPE *fmi2FreeInstance_;

        std::string getLastError() const;

    protected:
        DLL_HANDLE handle_ = nullptr;
        
    public:

        FmiLibrary(const std::string &modelIdentifier, const std::shared_ptr<FmuResource> &resource);

        fmi2String getVersion() const;

        fmi2String getTypesPlatform() const;

        fmi2Status setDebugLogging(fmi2Component c, bool loggingOn, const std::vector<char*> categories) const;

        fmi2Component instantiate(std::string instanceName, fmi2Type type,
                                  std::string guid, std::string resourceLocation,
                                  bool visible = false, bool loggingOn = false);

        fmi2Status setupExperiment(fmi2Component c, bool toleranceDefined,
                                   double tolerance, double startTime, double stopTime) const;

        fmi2Status enterInitializationMode(fmi2Component c) const;

        fmi2Status exitInitializationMode(fmi2Component c) const;

        fmi2Status reset(fmi2Component c) const;

        fmi2Status terminate(fmi2Component c);

        fmi2Status readInteger(fmi2Component c, fmi2ValueReference vr, fmi2Integer &ref) const;

        fmi2Status
        readInteger(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) const;

        fmi2Status readReal(fmi2Component c, fmi2ValueReference vr, fmi2Real &ref) const;

        fmi2Status readReal(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) const;

        fmi2Status readString(fmi2Component c, fmi2ValueReference vr, fmi2String &ref) const;

        fmi2Status
        readString(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) const;

        fmi2Status readBoolean(fmi2Component c, fmi2ValueReference vr, fmi2Boolean &ref) const;

        fmi2Status
        readBoolean(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) const;

        fmi2Status writeInteger(fmi2Component c, fmi2ValueReference vr, const fmi2Integer &value) const;

        fmi2Status writeInteger(fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                                const std::vector<fmi2Integer> &values) const;

        fmi2Status writeReal(fmi2Component c, fmi2ValueReference vr, const fmi2Real &value) const;

        fmi2Status
        writeReal(fmi2Component c, const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real> &values) const;

        fmi2Status writeString(fmi2Component c, fmi2ValueReference vr, fmi2String &value) const;

        fmi2Status writeString(fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                               const std::vector<fmi2String> &values) const;

        fmi2Status writeBoolean(fmi2Component c, fmi2ValueReference vr, const fmi2Boolean &value) const;

        fmi2Status writeBoolean(fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                                const std::vector<fmi2Boolean> &values) const;

        fmi2Status getFMUstate(fmi2Component c, fmi2FMUstate &state) const;

        fmi2Status setFMUstate(fmi2Component c, fmi2FMUstate state) const;

        fmi2Status freeFMUstate(fmi2Component c, fmi2FMUstate &state) const;

        fmi2Status
        getSerializedFMUstateSize(fmi2Component c, fmi2FMUstate state, size_t &size) const;

        fmi2Status
        serializeFMUstate(fmi2Component c, const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) const;

        fmi2Status
        deSerializeFMUstate(fmi2Component c, fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) const;

        fmi2Status getDirectionalDerivative(fmi2Component c,
                                            const std::vector<fmi2ValueReference> &vUnkownRef,
                                            const std::vector<fmi2ValueReference> &vKnownRef,
                                            const std::vector<fmi2Real> &dvKnownRef, std::vector<fmi2Real> &dvUnknownRef) const;

        void freeInstance(fmi2Component c);

        ~FmiLibrary();

    };

}

#endif //FMI4CPP_FMILIBRARY_HPP

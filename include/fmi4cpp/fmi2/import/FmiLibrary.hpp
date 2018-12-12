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

        fmi2Status lastStatus_;

        bool updateStatusAndReturnTrueIfOK(fmi2Status status);

    public:

        FmiLibrary(const std::string &modelIdentifier, const std::shared_ptr<FmuResource> &resource);

        fmi2Status getLastStatus() const;

        fmi2String getVersion() const;

        fmi2String getTypesPlatform() const;

        bool setDebugLogging(fmi2Component c, bool loggingOn, std::vector<char *> categories);

        fmi2Component instantiate(std::string instanceName, fmi2Type type,
                                  std::string guid, std::string resourceLocation,
                                  bool visible = false, bool loggingOn = false);

        bool setupExperiment(fmi2Component c, double tolerance, double startTime, double stopTime);

        bool enterInitializationMode(fmi2Component c);

        bool exitInitializationMode(fmi2Component c);

        bool reset(fmi2Component c);

        bool terminate(fmi2Component c);

        bool readInteger(fmi2Component c, fmi2ValueReference vr, fmi2Integer &ref);

        bool readInteger(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref);

        bool readReal(fmi2Component c, fmi2ValueReference vr, fmi2Real &ref);

        bool readReal(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref);

        bool readString(fmi2Component c, fmi2ValueReference vr, fmi2String &ref);

        bool readString(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref);

        bool readBoolean(fmi2Component c, fmi2ValueReference vr, fmi2Boolean &ref);

        bool readBoolean(fmi2Component c, const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref);

        bool writeInteger(fmi2Component c, fmi2ValueReference vr, const fmi2Integer &value);

        bool writeInteger(fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                          const std::vector<fmi2Integer> &values);

        bool writeReal(fmi2Component c, fmi2ValueReference vr, const fmi2Real &value);

        bool writeReal(fmi2Component c, const std::vector<fmi2ValueReference> &vr, const std::vector<fmi2Real> &values);

        bool writeString(fmi2Component c, fmi2ValueReference vr, fmi2String &value);

        bool writeString(fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                         const std::vector<fmi2String> &values);

        bool writeBoolean(fmi2Component c, fmi2ValueReference vr, const fmi2Boolean &value);

        bool writeBoolean(fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                          const std::vector<fmi2Boolean> &values);

        bool getFMUstate(fmi2Component c, fmi2FMUstate &state);

        bool setFMUstate(fmi2Component c, fmi2FMUstate state);

        bool freeFMUstate(fmi2Component c, fmi2FMUstate &state);

        bool getSerializedFMUstateSize(fmi2Component c, fmi2FMUstate state, size_t &size);

        bool serializeFMUstate(fmi2Component c, const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState);

        bool deSerializeFMUstate(fmi2Component c, fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState);

        bool getDirectionalDerivative(fmi2Component c,
                                      const std::vector<fmi2ValueReference> &vUnknownRef,
                                      const std::vector<fmi2ValueReference> &vKnownRef,
                                      const std::vector<fmi2Real> &dvKnownRef, std::vector<fmi2Real> &dvUnknownRef);

        void freeInstance(fmi2Component c);

        ~FmiLibrary();

    };

}

#endif //FMI4CPP_FMILIBRARY_HPP

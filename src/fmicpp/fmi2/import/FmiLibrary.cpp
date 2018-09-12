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

#include <sstream>
#include <iostream>
#include <fmicpp/fmi2/enumsToString.hpp>
#include <fmicpp/fmi2/import/FmiLibrary.hpp>

using namespace std;
using namespace fmicpp::fmi2::import;

namespace {

    void logger(void* fmi2ComponentEnvironment,
            fmi2String instance_name, fmi2Status status, fmi2String category, fmi2String message, ...) {
        printf("status = %s, instanceName = %s, category = %s: %s\n", to_string(status).c_str(), instance_name, category, message);
    }

    const fmi2CallbackFunctions callback = {
            logger,
            calloc,
            free,
            NULL,
            NULL
    };

}


FmiLibrary::FmiLibrary(const string &libName) {

#ifndef NDEBUG
    cout << "Loading shared library '" << libName << "'" << endl;
#endif

#ifdef WIN32
    handle_ = LoadLibrary(libName.c_str());
#else
    handle_ = dlopen(libName.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

    if (!handle_) {
        cerr << getLastError() << endl;
        string msg = "Unable to load dynamic library '" + libName + "'!";
        throw runtime_error(msg);
    }

}

string FmiLibrary::getLastError() const {
#ifdef WIN32
    std::ostringstream os;
    os << GetLastError();
    return os.str();
#else
    return dlerror();
#endif
}

fmi2String FmiLibrary::getVersion() const {
    return loadFunction<fmi2GetVersionTYPE *>("fmi2GetVersion")();
}

fmi2String FmiLibrary::getTypesPlatform() const {
    return loadFunction<fmi2GetTypesPlatformTYPE *>("fmi2GetTypesPlatform")();
}

fmi2Component FmiLibrary::instantiate(const string instanceName, const fmi2Type type,
        const string guid, const string resourceLocation, const bool visible, const bool loggingOn) {
    fmi2Component c = loadFunction<fmi2InstantiateTYPE *>("fmi2Instantiate")(instanceName.c_str(),
            type, guid.c_str(), resourceLocation.c_str(), &callback, visible ? 1 : 0, loggingOn ? 1 : 0);

    if (c == nullptr) {
        throw runtime_error("Unable to instantiate FMU instance!");
    }

    return c;
    
}

fmi2Status FmiLibrary::setupExperiment(const fmi2Component c, const bool toleranceDefined,
        const double tolerance, const double startTime, const double stopTime) const {

    fmi2Boolean stopDefined = (stopTime > startTime) ? 1 : 0;
    return loadFunction<fmi2SetupExperimentTYPE *>("fmi2SetupExperiment")
            (c, toleranceDefined ? 1 : 0, tolerance, startTime, stopDefined, stopTime);
}

fmi2Status FmiLibrary::enterInitializationMode(const fmi2Component c) const {
    return loadFunction<fmi2EnterInitializationModeTYPE *>("fmi2EnterInitializationMode")(c);
}

fmi2Status FmiLibrary::exitInitializationMode(const fmi2Component c) const {
    return loadFunction<fmi2ExitInitializationModeTYPE *>("fmi2ExitInitializationMode")(c);
}

fmi2Status FmiLibrary::reset(const fmi2Component c) const {
    return loadFunction<fmi2ResetTYPE *>("fmi2Reset")(c);
}

fmi2Status FmiLibrary::terminate(const fmi2Component c) {
    return loadFunction<fmi2TerminateTYPE *>("fmi2Terminate")(c);
}

fmi2Status FmiLibrary::readInteger(const fmi2Component c, const fmi2ValueReference vr, fmi2Integer &ref) const {
    return loadFunction<fmi2GetIntegerTYPE *>("fmi2GetInteger")(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readInteger(const fmi2Component c,
        const vector<fmi2ValueReference> &vr, vector<fmi2Integer> &ref) const {
    return loadFunction<fmi2GetIntegerTYPE *>("fmi2GetInteger")(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readReal(const fmi2Component c, const fmi2ValueReference vr, fmi2Real &ref) const {
    return loadFunction<fmi2GetRealTYPE *>("fmi2GetReal")(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readReal(const fmi2Component c,
        const vector<fmi2ValueReference > &vr, vector<fmi2Real> &ref) const {
    return loadFunction<fmi2GetRealTYPE *>("fmi2GetReal")(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readString(const fmi2Component c, const fmi2ValueReference vr, fmi2String &ref) const {
    return loadFunction<fmi2GetStringTYPE *>("fmi2GetString")(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readString(const fmi2Component c,
        const vector<fmi2ValueReference> &vr, vector<fmi2String > &ref) const {
    return loadFunction<fmi2GetStringTYPE *>("fmi2GetString")(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readBoolean(const fmi2Component c, const fmi2ValueReference vr, fmi2Boolean &ref) const {
    return loadFunction<fmi2GetBooleanTYPE *>("fmi2GetBoolean")(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readBoolean(const fmi2Component c,
        const vector<fmi2ValueReference> &vr, vector<fmi2Boolean> &ref) const {
    return loadFunction<fmi2GetBooleanTYPE *>("fmi2GetBoolean")(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::getFMUstate(const fmi2Component c, fmi2FMUstate& state) const {
    return loadFunction<fmi2GetFMUstateTYPE *>("fmi2GetFMUstate")(c, &state);
}

fmi2Status FmiLibrary::setFMUstate(const fmi2Component c, const fmi2FMUstate state) const {
    return loadFunction<fmi2SetFMUstateTYPE *>("fmi2SetFMUstate")(c, state);
}

fmi2Status FmiLibrary::freeFMUstate(const fmi2Component c, fmi2FMUstate& state) const {
    return loadFunction<fmi2FreeFMUstateTYPE *>("fmi2FreeFMUstate")(c, &state);
}

fmi2Status FmiLibrary::serializeFMUstate(const fmi2Component c,
        const fmi2FMUstate &state, vector<fmi2Byte> &serializedState) const {
    return loadFunction<fmi2SerializeFMUstateTYPE *>
            ("fmi2SerializeFMUstate")(c, state, serializedState.data(), serializedState.size());
}

fmi2Status FmiLibrary::deSerializeFMUstate(const fmi2Component c,
        fmi2FMUstate &state, const vector<fmi2Byte> &serializedState) const {
    return loadFunction<fmi2DeSerializeFMUstateTYPE *>
            ("fmi2DeSerializeFMUstate")(c, serializedState.data(), serializedState.size(), &state);
}

fmi2Status FmiLibrary::getDirectionalDerivative(const fmi2Component c,
        const vector<fmi2ValueReference> &vUnkownRef, const vector<fmi2ValueReference> &vKnownRef,
        const vector<fmi2Real> &dvKnownRef, vector<fmi2Real> &dvUnknownRef) const {
    return loadFunction<fmi2GetDirectionalDerivativeTYPE *>
            ("fmi2GetDirectionalDerivative")(c, vUnkownRef.data(), vUnkownRef.size(),
                    vKnownRef.data(), vKnownRef.size(), dvKnownRef.data(), dvUnknownRef.data());
}

void FmiLibrary::freeInstance(fmi2Component c) {
    loadFunction<fmi2FreeInstanceTYPE *>("fmi2FreeInstance")(c);
}

FmiLibrary::~FmiLibrary() {

   if (handle_) {
       bool success;
#ifdef WIN32
       success = FreeLibrary(handle_);
#else
       success = (dlclose(handle_) == 0);
#endif
        if (!success) {
            cout << getLastError() << endl;
        }
        handle_ = nullptr;
   }
}

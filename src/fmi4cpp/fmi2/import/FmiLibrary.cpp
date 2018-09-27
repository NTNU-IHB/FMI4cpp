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


#if FMI4CPP_DEBUG_LOGGING_ENABLED
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#include <sstream>
#include <iostream>
#include <fmi4cpp/fmi2/enumsToString.hpp>
#include <fmi4cpp/fmi2/import/FmiLibrary.hpp>

#include "FmiLibraryHelper.hpp"

using namespace std;
using fmi4cpp::fmi2::import::FmiLibrary;

namespace {

    void logger(void *fmi2ComponentEnvironment,
                fmi2String instance_name, fmi2Status status, fmi2String category, fmi2String message, ...) {
        printf("status = %s, instanceName = %s, category = %s: %s\n", to_string(status).c_str(), instance_name,
               category, message);
    }

    const fmi2CallbackFunctions callback = {
            logger, calloc, free, NULL, NULL
    };

}


FmiLibrary::FmiLibrary(const string &libName) {

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "Loading shared library " << fs::path(libName).stem() << endl;
#endif

    handle_ = loadLibrary(libName);

    if (!handle_) {
        cerr << getLastError() << endl;
        string msg = "Unable to load dynamic library '" + libName + "'!";
        throw runtime_error(msg);
    }

    fmi2GetVersion_ = loadFunction<fmi2GetVersionTYPE *>(handle_, "fmi2GetVersion");
    fmi2GetTypesPlatform_ = loadFunction<fmi2GetTypesPlatformTYPE *>(handle_, "fmi2GetTypesPlatform");

    fmi2SetDebugLogging_ = loadFunction<fmi2SetDebugLoggingTYPE *>(handle_, "fmi2SetDebugLogging");

    fmi2Instantiate_ = loadFunction<fmi2InstantiateTYPE *>(handle_, "fmi2Instantiate");
    fmi2SetupExperiment_ = loadFunction<fmi2SetupExperimentTYPE *>(handle_, "fmi2SetupExperiment");
    fmi2EnterInitializationMode_ = loadFunction<fmi2EnterInitializationModeTYPE *>(handle_, "fmi2EnterInitializationMode");
    fmi2ExitInitializationMode_ = loadFunction<fmi2ExitInitializationModeTYPE *>(handle_, "fmi2ExitInitializationMode");

    fmi2Reset_ = loadFunction<fmi2ResetTYPE *>(handle_, "fmi2Reset");
    fmi2Terminate_ = loadFunction<fmi2TerminateTYPE *>(handle_, "fmi2Terminate");

    fmi2GetInteger_ = loadFunction<fmi2GetIntegerTYPE *>(handle_, "fmi2GetInteger");
    fmi2GetReal_ = loadFunction<fmi2GetRealTYPE *>(handle_, "fmi2GetReal");
    fmi2GetString_ = loadFunction<fmi2GetStringTYPE *>(handle_, "fmi2GetString");
    fmi2GetBoolean_ = loadFunction<fmi2GetBooleanTYPE *>(handle_, "fmi2GetBoolean");

    fmi2SetInteger_ = loadFunction<fmi2SetIntegerTYPE *>(handle_, "fmi2SetInteger");
    fmi2SetReal_ = loadFunction<fmi2SetRealTYPE *>(handle_, "fmi2SetReal");
    fmi2SetString_ = loadFunction<fmi2SetStringTYPE *>(handle_, "fmi2SetString");
    fmi2SetBoolean_ = loadFunction<fmi2SetBooleanTYPE *>(handle_, "fmi2SetBoolean");

    fmi2GetFMUstate_ = loadFunction<fmi2GetFMUstateTYPE *>(handle_, "fmi2GetFMUstate");
    fmi2SetFMUstate_ = loadFunction<fmi2SetFMUstateTYPE *>(handle_, "fmi2SetFMUstate");
    fmi2FreeFMUstate_ = loadFunction<fmi2FreeFMUstateTYPE *>(handle_, "fmi2FreeFMUstate");
    fmi2SerializeFMUstate_ = loadFunction<fmi2SerializeFMUstateTYPE *>(handle_, "fmi2SerializeFMUstate");
    fmi2DeSerializeFMUstate_ = loadFunction<fmi2DeSerializeFMUstateTYPE *>(handle_, "fmi2DeSerializeFMUstate");

    fmi2GetDirectionalDerivative_ = loadFunction<fmi2GetDirectionalDerivativeTYPE *>(handle_, "fmi2GetDirectionalDerivative");

    fmi2FreeInstance_ = loadFunction<fmi2FreeInstanceTYPE *>(handle_, "fmi2FreeInstance");

}

fmi2String FmiLibrary::getVersion() const {
    return fmi2GetVersion_();
}

fmi2String FmiLibrary::getTypesPlatform() const {
    return fmi2GetTypesPlatform_();
}

fmi2Status fmi4cpp::fmi2::import::FmiLibrary::setDebugLogging(const fmi2Component c, const bool loggingOn,
                                                                  const vector<const char*> categories) const {
    return fmi2SetDebugLogging_(c, loggingOn, categories.size(), categories.data());
}

fmi2Component FmiLibrary::instantiate(const string instanceName, const fmi2Type type,
                                      const string guid, const string resourceLocation, const bool visible,
                                      const bool loggingOn) {
    fmi2Component c = fmi2Instantiate_(instanceName.c_str(), type, guid.c_str(),
                                       resourceLocation.c_str(), &callback, visible ? 1 : 0, loggingOn ? 1 : 0);

    if (c == nullptr) {
        throw runtime_error("Unable to instantiate FMU instance!");
    }

    return c;

}

fmi2Status FmiLibrary::setupExperiment(const fmi2Component c, const bool toleranceDefined,
                                       const double tolerance, const double startTime, const double stopTime) const {

    fmi2Boolean stopDefined = (stopTime > startTime) ? 1 : 0;
    return fmi2SetupExperiment_(c, toleranceDefined ? 1 : 0, tolerance, startTime, stopDefined, stopTime);
}

fmi2Status FmiLibrary::enterInitializationMode(const fmi2Component c) const {
    return fmi2EnterInitializationMode_(c);
}

fmi2Status FmiLibrary::exitInitializationMode(const fmi2Component c) const {
    return fmi2ExitInitializationMode_(c);
}

fmi2Status FmiLibrary::reset(const fmi2Component c) const {
    return fmi2Reset_(c);
}

fmi2Status FmiLibrary::terminate(const fmi2Component c) {
    return fmi2Terminate_(c);
}

fmi2Status FmiLibrary::readInteger(const fmi2Component c, const fmi2ValueReference vr, fmi2Integer &ref) const {
    return fmi2GetInteger_(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readInteger(const fmi2Component c,
                                   const vector<fmi2ValueReference> &vr, vector<fmi2Integer> &ref) const {
    return fmi2GetInteger_(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readReal(const fmi2Component c, const fmi2ValueReference vr, fmi2Real &ref) const {
    return fmi2GetReal_(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readReal(const fmi2Component c,
                                const vector<fmi2ValueReference> &vr, vector<fmi2Real> &ref) const {
    return fmi2GetReal_(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readString(const fmi2Component c, const fmi2ValueReference vr, fmi2String &ref) const {
    return fmi2GetString_(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readString(const fmi2Component c,
                                  const vector<fmi2ValueReference> &vr, vector<fmi2String> &ref) const {
    return fmi2GetString_(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readBoolean(const fmi2Component c, const fmi2ValueReference vr, fmi2Boolean &ref) const {
    return fmi2GetBoolean_(c, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readBoolean(const fmi2Component c,
                                   const vector<fmi2ValueReference> &vr, vector<fmi2Boolean> &ref) const {
    return fmi2GetBoolean_(c, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::writeInteger(const fmi2Component c, const fmi2ValueReference vr,
                                    const fmi2Integer &value) const {
    return fmi2SetInteger_(c, &vr, 1, &value);
}

fmi2Status FmiLibrary::writeInteger(const fmi2Component c, const vector<fmi2ValueReference> &vr,
                                    const vector<fmi2Integer> &values) const {
    return fmi2SetInteger_(c, vr.data(), vr.size(), values.data());
}

fmi2Status FmiLibrary::writeReal(const fmi2Component c, const fmi2ValueReference vr, const fmi2Real &value) const {
    return fmi2SetReal_(c, &vr, 1, &value);
}

fmi2Status FmiLibrary::writeReal(const fmi2Component c, const vector<fmi2ValueReference> &vr,
                                 const vector<fmi2Real> &values) const {
    return fmi2SetReal_(c, vr.data(), vr.size(), values.data());
}

fmi2Status FmiLibrary::writeString(const fmi2Component c, const fmi2ValueReference vr,
                                   fmi2String &value) const {
    return fmi2SetString_(c, &vr, 1, &value);
}

fmi2Status FmiLibrary::writeString(const fmi2Component c, const vector<fmi2ValueReference> &vr,
                                   const vector<fmi2String> &values) const {
    return fmi2SetString_(c, vr.data(), vr.size(), values.data());
}

fmi2Status FmiLibrary::writeBoolean(const fmi2Component c, const fmi2ValueReference vr,
                                    const fmi2Boolean &value) const {
    return fmi2SetBoolean_(c, &vr, 1, &value);
}

fmi2Status FmiLibrary::writeBoolean(const fmi2Component c, const vector<fmi2ValueReference> &vr,
                                    const vector<fmi2Boolean> &values) const {
    return fmi2SetBoolean_(c, vr.data(), vr.size(), values.data());
}

fmi2Status FmiLibrary::getFMUstate(const fmi2Component c, fmi2FMUstate &state) const {
    return fmi2GetFMUstate_(c, &state);
}

fmi2Status FmiLibrary::setFMUstate(const fmi2Component c, const fmi2FMUstate state) const {
    return fmi2SetFMUstate_(c, state);
}

fmi2Status FmiLibrary::freeFMUstate(const fmi2Component c, fmi2FMUstate &state) const {
    return fmi2FreeFMUstate_(c, &state);
}

fmi2Status fmi4cpp::fmi2::import::FmiLibrary::getSerializedFMUstateSize(const fmi2Component c, const fmi2FMUstate state,
                                                                        size_t &size) const {
    return fmi2SerializedFMUstateSize_(c, state, &size);
}

fmi2Status FmiLibrary::serializeFMUstate(const fmi2Component c,
                                         const fmi2FMUstate &state, vector<fmi2Byte> &serializedState) const {
    size_t size = 0;
    getSerializedFMUstateSize(c, state, size);
    serializedState.reserve(size);
    return fmi2SerializeFMUstate_(c, state, serializedState.data(), size);
}

fmi2Status FmiLibrary::deSerializeFMUstate(const fmi2Component c,
                                           fmi2FMUstate &state, const vector<fmi2Byte> &serializedState) const {
    return fmi2DeSerializeFMUstate_(c, serializedState.data(), serializedState.size(), &state);
}

fmi2Status FmiLibrary::getDirectionalDerivative(const fmi2Component c,
                                                const vector<fmi2ValueReference> &vUnkownRef,
                                                const vector<fmi2ValueReference> &vKnownRef,
                                                const vector<fmi2Real> &dvKnownRef,
                                                vector<fmi2Real> &dvUnknownRef) const {
    return fmi2GetDirectionalDerivative_(c, vUnkownRef.data(), vUnkownRef.size(),
                                         vKnownRef.data(), vKnownRef.size(), dvKnownRef.data(), dvUnknownRef.data());
}

void FmiLibrary::freeInstance(fmi2Component c) {
    fmi2FreeInstance_(c);
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

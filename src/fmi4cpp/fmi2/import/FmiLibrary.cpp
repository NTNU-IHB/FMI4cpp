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

#include <fmi4cpp/fmi2/import/FmiLibrary.hpp>

#include "FmiLibraryHelper.hpp"

using namespace fmi4cpp::fmi2;

namespace {

    const std::string to_string(fmi2Status status) {

        switch (status) {
            case fmi2OK:
                return "OK";
            case fmi2Warning:
                return "Warning";
            case fmi2Discard:
                return "Discard";
            case fmi2Error:
                return "Error";
            case fmi2Fatal:
                return "Fatal";
            case fmi2Pending:
                return "Pending";
            default:
                return "Unknown";
        }

    }

    void logger(void *fmi2ComponentEnvironment,
                fmi2String instance_name, fmi2Status status, fmi2String category, fmi2String message, ...) {
        printf("status = %s, instanceName = %s, category = %s: %s\n", to_string(status).c_str(), instance_name,
               category, message);
    }

    const fmi2CallbackFunctions callback = {
            logger, calloc, free, nullptr, nullptr
    };

}


FmiLibrary::FmiLibrary(const std::string &modelIdentifier, const std::shared_ptr<FmuResource> &resource)
        : resource_(resource) {

    const auto libName = resource->getAbsoluteLibraryPath(modelIdentifier);

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    std::cout << "Loading shared library '" << fs::path(libName).stem() << "'" << std::endl;
#endif

    handle_ = loadLibrary(libName);

    if (!handle_) {
        std::cerr << getLastError() << std::endl;
        throw std::runtime_error("Unable to load dynamic library '" + libName + "'!");
    }

    fmi2GetVersion_ = loadFunction<fmi2GetVersionTYPE *>(handle_, "fmi2GetVersion");
    fmi2GetTypesPlatform_ = loadFunction<fmi2GetTypesPlatformTYPE *>(handle_, "fmi2GetTypesPlatform");

    fmi2SetDebugLogging_ = loadFunction<fmi2SetDebugLoggingTYPE *>(handle_, "fmi2SetDebugLogging");

    fmi2Instantiate_ = loadFunction<fmi2InstantiateTYPE *>(handle_, "fmi2Instantiate");
    fmi2SetupExperiment_ = loadFunction<fmi2SetupExperimentTYPE *>(handle_, "fmi2SetupExperiment");
    fmi2EnterInitializationMode_ = loadFunction<fmi2EnterInitializationModeTYPE *>(handle_,
                                                                                   "fmi2EnterInitializationMode");
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

    fmi2GetDirectionalDerivative_ = loadFunction<fmi2GetDirectionalDerivativeTYPE *>(handle_,
                                                                                     "fmi2GetDirectionalDerivative");

    fmi2FreeInstance_ = loadFunction<fmi2FreeInstanceTYPE *>(handle_, "fmi2FreeInstance");

}

bool FmiLibrary::updateStatusAndReturnTrueIfOK(fmi2Status status) {
    lastStatus_ = status;
    return status == fmi2OK;
}

fmi2Status FmiLibrary::getLastStatus() const {
    return lastStatus_;
}

fmi2String FmiLibrary::getVersion() const {
    return fmi2GetVersion_();
}

fmi2String FmiLibrary::getTypesPlatform() const {
    return fmi2GetTypesPlatform_();
}

fmi2Component FmiLibrary::instantiate(const std::string instanceName, const fmi2Type type,
                                      const std::string guid, const std::string resourceLocation, 
                                      bool visible, bool loggingOn) {
    fmi2Component c = fmi2Instantiate_(instanceName.c_str(), type, guid.c_str(),
                                       resourceLocation.c_str(), &callback, visible, loggingOn);

    if (c == nullptr) {
        throw std::runtime_error("Fatal: fmi2Instantiate returned nullptr, unable to instantiate FMU instance!");
    }

    return c;

}

bool FmiLibrary::setDebugLogging(fmi2Component c, bool loggingOn,
                                 const std::vector<char *> categories) {
    return updateStatusAndReturnTrueIfOK(fmi2SetDebugLogging_(c, loggingOn, categories.size(), categories.data()));
}

bool FmiLibrary::setupExperiment(const fmi2Component c, double tolerance, double startTime, double stopTime) {

    bool stopDefined = (stopTime > startTime);
    bool toleranceDefined = (tolerance > 0);
    return updateStatusAndReturnTrueIfOK(fmi2SetupExperiment_(c, toleranceDefined, tolerance, startTime, stopDefined, stopTime));
}

bool FmiLibrary::enterInitializationMode(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2EnterInitializationMode_(c));
}

bool FmiLibrary::exitInitializationMode(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2ExitInitializationMode_(c));
}

bool FmiLibrary::reset(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2Reset_(c));
}

bool FmiLibrary::terminate(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2Terminate_(c));
}

bool FmiLibrary::readInteger(const fmi2Component c, const fmi2ValueReference vr, fmi2Integer &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetInteger_(c, &vr, 1, &ref));
}

bool FmiLibrary::readInteger(const fmi2Component c,
                                   const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetInteger_(c, vr.data(), vr.size(), ref.data()));
}

bool FmiLibrary::readReal(const fmi2Component c, const fmi2ValueReference vr, fmi2Real &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetReal_(c, &vr, 1, &ref));
}

bool FmiLibrary::readReal(const fmi2Component c,
                                const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetReal_(c, vr.data(), vr.size(), ref.data()));
}

bool FmiLibrary::readString(const fmi2Component c, const fmi2ValueReference vr, fmi2String &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetString_(c, &vr, 1, &ref));
}

bool FmiLibrary::readString(const fmi2Component c,
                                  const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetString_(c, vr.data(), vr.size(), ref.data()));
}

bool FmiLibrary::readBoolean(const fmi2Component c, const fmi2ValueReference vr, fmi2Boolean &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetBoolean_(c, &vr, 1, &ref));
}

bool FmiLibrary::readBoolean(const fmi2Component c,
                                   const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetBoolean_(c, vr.data(), vr.size(), ref.data()));
}

bool FmiLibrary::writeInteger(const fmi2Component c, const fmi2ValueReference vr,
                                    const fmi2Integer &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetInteger_(c, &vr, 1, &value));
}

bool FmiLibrary::writeInteger(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                                    const std::vector<fmi2Integer> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetInteger_(c, vr.data(), vr.size(), values.data()));
}

bool FmiLibrary::writeReal(const fmi2Component c, const fmi2ValueReference vr, const fmi2Real &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetReal_(c, &vr, 1, &value));
}

bool FmiLibrary::writeReal(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                                 const std::vector<fmi2Real> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetReal_(c, vr.data(), vr.size(), values.data()));
}

bool FmiLibrary::writeString(const fmi2Component c, const fmi2ValueReference vr,
                                   fmi2String &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetString_(c, &vr, 1, &value));
}

bool FmiLibrary::writeString(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                                   const std::vector<fmi2String> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetString_(c, vr.data(), vr.size(), values.data()));
}

bool FmiLibrary::writeBoolean(const fmi2Component c, const fmi2ValueReference vr,
                                    const fmi2Boolean &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetBoolean_(c, &vr, 1, &value));
}

bool FmiLibrary::writeBoolean(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                                    const std::vector<fmi2Boolean> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetBoolean_(c, vr.data(), vr.size(), values.data()));
}

bool FmiLibrary::getFMUstate(const fmi2Component c, fmi2FMUstate &state) {
    return updateStatusAndReturnTrueIfOK(fmi2GetFMUstate_(c, &state)) ;
}

bool FmiLibrary::setFMUstate(const fmi2Component c, const fmi2FMUstate state) {
    return updateStatusAndReturnTrueIfOK(fmi2SetFMUstate_(c, state));
}

bool FmiLibrary::freeFMUstate(const fmi2Component c, fmi2FMUstate &state) {
    return updateStatusAndReturnTrueIfOK(fmi2FreeFMUstate_(c, &state));
}

bool fmi4cpp::fmi2::FmiLibrary::getSerializedFMUstateSize(const fmi2Component c, const fmi2FMUstate state,
                                                                size_t &size) {
    return updateStatusAndReturnTrueIfOK(fmi2SerializedFMUstateSize_(c, state, &size));
}

bool FmiLibrary::serializeFMUstate(const fmi2Component c,
                                         const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) {
    size_t size = 0;
    getSerializedFMUstateSize(c, state, size);
    serializedState.reserve(size);
    return updateStatusAndReturnTrueIfOK(fmi2SerializeFMUstate_(c, state, serializedState.data(), size));
}

bool FmiLibrary::deSerializeFMUstate(const fmi2Component c,
                                           fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) {
    return updateStatusAndReturnTrueIfOK(fmi2DeSerializeFMUstate_(c, serializedState.data(), serializedState.size(), &state));
}

bool FmiLibrary::getDirectionalDerivative(const fmi2Component c,
                                                const std::vector<fmi2ValueReference> &vUnknownRef,
                                                const std::vector<fmi2ValueReference> &vKnownRef,
                                                const std::vector<fmi2Real> &dvKnownRef,
                                                std::vector<fmi2Real> &dvUnknownRef) {
    return updateStatusAndReturnTrueIfOK(fmi2GetDirectionalDerivative_(c, vUnknownRef.data(), vUnknownRef.size(),
                                         vKnownRef.data(), vKnownRef.size(), dvKnownRef.data(), dvUnknownRef.data()));
}

void FmiLibrary::freeInstance(fmi2Component c) {
    fmi2FreeInstance_(c);
}

std::string FmiLibrary::getLastError() const {
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
        success = static_cast<bool>(FreeLibrary(handle_));
#else
        success = (dlclose(handle_) == 0);
#endif

#if FMI4CPP_DEBUG_LOGGING_ENABLED
        if (!success) {
            std::cout << getLastError() << std::endl;
        }
#endif

        handle_ = nullptr;
    }
}

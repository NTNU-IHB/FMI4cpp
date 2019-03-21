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

#include <fmi4cpp/fmi2/library.hpp>
#include <fmi4cpp/fmi2/library_helper.hpp>

#include <fmi4cpp/logger.hpp>
#include <fmi4cpp/tools/os_util.hpp>


using namespace fmi4cpp;
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

        FMI4CPP_INFO("[FMI callback logger] status=" + to_string(status) + ", instanceName=" + instance_name +
                      ", category=" + category + ", message=" + message);

    }

    const fmi2CallbackFunctions callback = {
            logger, calloc, free, nullptr, nullptr
    };

}


fmi2Library::fmi2Library(const std::string &modelIdentifier, const std::shared_ptr<fmu_resource> &resource)
        : resource_(resource) {

    const auto libName = resource->absolute_library_path(modelIdentifier);

    FMI4CPP_DEBUG("Loading shared library '" + fs::path(libName).stem().string() + getLibExt() + "'");

    handle_ = load_library(libName);

    if (!handle_) {
        FMI4CPP_ERROR("Unable to load dynamic library '" + libName + "':" + getLastError());
        throw std::runtime_error("Unable to load dynamic library '" + libName + "'!");
    }

    fmi2GetVersion_ = load_function<fmi2GetVersionTYPE *>(handle_, "fmi2GetVersion");
    fmi2GetTypesPlatform_ = load_function<fmi2GetTypesPlatformTYPE *>(handle_, "fmi2GetTypesPlatform");

    fmi2SetDebugLogging_ = load_function<fmi2SetDebugLoggingTYPE *>(handle_, "fmi2SetDebugLogging");

    fmi2Instantiate_ = load_function<fmi2InstantiateTYPE *>(handle_, "fmi2Instantiate");
    fmi2SetupExperiment_ = load_function<fmi2SetupExperimentTYPE *>(handle_, "fmi2SetupExperiment");
    fmi2EnterInitializationMode_ = load_function<fmi2EnterInitializationModeTYPE *>(handle_,
                                                                                    "fmi2EnterInitializationMode");
    fmi2ExitInitializationMode_ = load_function<fmi2ExitInitializationModeTYPE *>(handle_,
                                                                                  "fmi2ExitInitializationMode");

    fmi2Reset_ = load_function<fmi2ResetTYPE *>(handle_, "fmi2Reset");
    fmi2Terminate_ = load_function<fmi2TerminateTYPE *>(handle_, "fmi2Terminate");

    fmi2GetInteger_ = load_function<fmi2GetIntegerTYPE *>(handle_, "fmi2GetInteger");
    fmi2GetReal_ = load_function<fmi2GetRealTYPE *>(handle_, "fmi2GetReal");
    fmi2GetString_ = load_function<fmi2GetStringTYPE *>(handle_, "fmi2GetString");
    fmi2GetBoolean_ = load_function<fmi2GetBooleanTYPE *>(handle_, "fmi2GetBoolean");

    fmi2SetInteger_ = load_function<fmi2SetIntegerTYPE *>(handle_, "fmi2SetInteger");
    fmi2SetReal_ = load_function<fmi2SetRealTYPE *>(handle_, "fmi2SetReal");
    fmi2SetString_ = load_function<fmi2SetStringTYPE *>(handle_, "fmi2SetString");
    fmi2SetBoolean_ = load_function<fmi2SetBooleanTYPE *>(handle_, "fmi2SetBoolean");

    fmi2GetFMUstate_ = load_function<fmi2GetFMUstateTYPE *>(handle_, "fmi2GetFMUstate");
    fmi2SetFMUstate_ = load_function<fmi2SetFMUstateTYPE *>(handle_, "fmi2SetFMUstate");
    fmi2FreeFMUstate_ = load_function<fmi2FreeFMUstateTYPE *>(handle_, "fmi2FreeFMUstate");
    fmi2SerializeFMUstate_ = load_function<fmi2SerializeFMUstateTYPE *>(handle_, "fmi2SerializeFMUstate");
    fmi2DeSerializeFMUstate_ = load_function<fmi2DeSerializeFMUstateTYPE *>(handle_, "fmi2DeSerializeFMUstate");

    fmi2GetDirectionalDerivative_ = load_function<fmi2GetDirectionalDerivativeTYPE *>(handle_,
                                                                                      "fmi2GetDirectionalDerivative");

    fmi2FreeInstance_ = load_function<fmi2FreeInstanceTYPE *>(handle_, "fmi2FreeInstance");

}

bool fmi2Library::updateStatusAndReturnTrueIfOK(fmi2Status status) {
    lastStatus_ = status;
    return status == fmi2OK;
}

fmi2Status fmi2Library::getLastStatus() const {
    return lastStatus_;
}

fmi2String fmi2Library::getVersion() const {
    return fmi2GetVersion_();
}

fmi2String fmi2Library::getTypesPlatform() const {
    return fmi2GetTypesPlatform_();
}

fmi2Component fmi2Library::instantiate(const std::string &instanceName, const fmi2Type type,
                                       const std::string &guid, const std::string &resourceLocation,
                                       bool visible, bool loggingOn) {
    fmi2Component c = fmi2Instantiate_(instanceName.c_str(), type, guid.c_str(),
                                       resourceLocation.c_str(), &callback, visible, loggingOn);

    if (c == nullptr) {
        const std::string msg = "Fatal: fmi2Instantiate returned nullptr, unable to instantiate FMU instance!";
        FMI4CPP_ERROR(msg);
        throw std::runtime_error(msg);
    }

    return c;

}

bool fmi2Library::setDebugLogging(fmi2Component c, bool loggingOn,
                                  const std::vector<fmi2String> categories) {
    return updateStatusAndReturnTrueIfOK(fmi2SetDebugLogging_(c, loggingOn, categories.size(), categories.data()));
}

bool fmi2Library::setupExperiment(const fmi2Component c, double tolerance, double startTime, double stopTime) {

    bool stopDefined = (stopTime > startTime);
    bool toleranceDefined = (tolerance > 0);
    return updateStatusAndReturnTrueIfOK(
            fmi2SetupExperiment_(c, toleranceDefined, tolerance, startTime, stopDefined, stopTime));
}

bool fmi2Library::enterInitializationMode(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2EnterInitializationMode_(c));
}

bool fmi2Library::exitInitializationMode(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2ExitInitializationMode_(c));
}

bool fmi2Library::reset(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2Reset_(c));
}

bool fmi2Library::terminate(const fmi2Component c) {
    return updateStatusAndReturnTrueIfOK(fmi2Terminate_(c));
}

bool fmi2Library::readInteger(const fmi2Component c, const fmi2ValueReference vr, fmi2Integer &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetInteger_(c, &vr, 1, &ref));
}

bool fmi2Library::readInteger(const fmi2Component c,
                              const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Integer> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetInteger_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2Library::readReal(const fmi2Component c, const fmi2ValueReference vr, fmi2Real &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetReal_(c, &vr, 1, &ref));
}

bool fmi2Library::readReal(const fmi2Component c,
                           const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Real> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetReal_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2Library::readString(const fmi2Component c, const fmi2ValueReference vr, fmi2String &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetString_(c, &vr, 1, &ref));
}

bool fmi2Library::readString(const fmi2Component c,
                             const std::vector<fmi2ValueReference> &vr, std::vector<fmi2String> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetString_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2Library::readBoolean(const fmi2Component c, const fmi2ValueReference vr, fmi2Boolean &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetBoolean_(c, &vr, 1, &ref));
}

bool fmi2Library::readBoolean(const fmi2Component c,
                              const std::vector<fmi2ValueReference> &vr, std::vector<fmi2Boolean> &ref) {
    return updateStatusAndReturnTrueIfOK(fmi2GetBoolean_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2Library::writeInteger(const fmi2Component c, const fmi2ValueReference vr,
                               const fmi2Integer &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetInteger_(c, &vr, 1, &value));
}

bool fmi2Library::writeInteger(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                               const std::vector<fmi2Integer> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetInteger_(c, vr.data(), vr.size(), values.data()));
}

bool fmi2Library::writeReal(const fmi2Component c, const fmi2ValueReference vr, const fmi2Real &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetReal_(c, &vr, 1, &value));
}

bool fmi2Library::writeReal(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                            const std::vector<fmi2Real> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetReal_(c, vr.data(), vr.size(), values.data()));
}

bool fmi2Library::writeString(const fmi2Component c, const fmi2ValueReference vr,
                              fmi2String &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetString_(c, &vr, 1, &value));
}

bool fmi2Library::writeString(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                              const std::vector<fmi2String> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetString_(c, vr.data(), vr.size(), values.data()));
}

bool fmi2Library::writeBoolean(const fmi2Component c, const fmi2ValueReference vr,
                               const fmi2Boolean &value) {
    return updateStatusAndReturnTrueIfOK(fmi2SetBoolean_(c, &vr, 1, &value));
}

bool fmi2Library::writeBoolean(const fmi2Component c, const std::vector<fmi2ValueReference> &vr,
                               const std::vector<fmi2Boolean> &values) {
    return updateStatusAndReturnTrueIfOK(fmi2SetBoolean_(c, vr.data(), vr.size(), values.data()));
}

bool fmi2Library::getFMUstate(const fmi2Component c, fmi2FMUstate &state) {
    return updateStatusAndReturnTrueIfOK(fmi2GetFMUstate_(c, &state));
}

bool fmi2Library::setFMUstate(const fmi2Component c, const fmi2FMUstate state) {
    return updateStatusAndReturnTrueIfOK(fmi2SetFMUstate_(c, state));
}

bool fmi2Library::freeFMUstate(const fmi2Component c, fmi2FMUstate &state) {
    return updateStatusAndReturnTrueIfOK(fmi2FreeFMUstate_(c, &state));
}

bool fmi4cpp::fmi2::fmi2Library::getSerializedFMUstateSize(const fmi2Component c, const fmi2FMUstate state,
                                                           size_t &size) {
    return updateStatusAndReturnTrueIfOK(fmi2SerializedFMUstateSize_(c, state, &size));
}

bool fmi2Library::serializeFMUstate(const fmi2Component c,
                                    const fmi2FMUstate &state, std::vector<fmi2Byte> &serializedState) {
    size_t size = 0;
    getSerializedFMUstateSize(c, state, size);
    serializedState.reserve(size);
    return updateStatusAndReturnTrueIfOK(fmi2SerializeFMUstate_(c, state, serializedState.data(), size));
}

bool fmi2Library::deSerializeFMUstate(const fmi2Component c,
                                      fmi2FMUstate &state, const std::vector<fmi2Byte> &serializedState) {
    return updateStatusAndReturnTrueIfOK(
            fmi2DeSerializeFMUstate_(c, serializedState.data(), serializedState.size(), &state));
}

bool fmi2Library::getDirectionalDerivative(const fmi2Component c,
                                           const std::vector<fmi2ValueReference> &vUnknownRef,
                                           const std::vector<fmi2ValueReference> &vKnownRef,
                                           const std::vector<fmi2Real> &dvKnownRef,
                                           std::vector<fmi2Real> &dvUnknownRef) {
    return updateStatusAndReturnTrueIfOK(fmi2GetDirectionalDerivative_(c, vUnknownRef.data(), vUnknownRef.size(),
                                                                       vKnownRef.data(), vKnownRef.size(),
                                                                       dvKnownRef.data(), dvUnknownRef.data()));
}

void fmi2Library::freeInstance(fmi2Component c) {
    fmi2FreeInstance_(c);
}

std::string fmi2Library::getLastError() const {
#ifdef WIN32
    std::ostringstream os;
    os << GetLastError();
    return os.str();
#else
    return dlerror();
#endif
}

fmi2Library::~fmi2Library() {

    if (handle_) {
        bool success;
#ifdef WIN32
        success = static_cast<bool>(FreeLibrary(handle_));
#else
        success = (dlclose(handle_) == 0);
#endif

        if (!success) {
            FMI4CPP_ERROR(getLastError());
        }

        handle_ = nullptr;
    }
}

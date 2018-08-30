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

#include <iostream>
#include <fmicpp/fmi2/import/FmiLibrary.hpp>
#include <sstream>

using namespace std;
using namespace fmicpp::fmi2::import;

namespace {

    static const char* status_to_string(fmi2Status status) {
        switch (status){
            case 0: return "OK";
            case 1: return "Warning";
            case 2: return "Discard";
            case 3: return "Error";
            case 4: return "Fatal";
            case 5: return "Pending";
            default: return "Unknown";
        }
    }

    static void logger(void* fmi2ComponentEnvironment,
            fmi2String instance_name, fmi2Status status, fmi2String category, fmi2String message, ...) {
        printf("status = %s, instanceName = %s, category = %s: %s\n", status_to_string(status), instance_name, category, message);
    }

    static const fmi2CallbackFunctions callback = {
            logger,
            calloc,
            free,
            NULL,
            NULL
    };

}

const char *FmiLibrary::getLastError() const {
#ifdef WIN32
    std::ostringstream os;
    os << GetLastError();
    return os.str().c_str();
#else
    return dlerror();
#endif
}

FmiLibrary::FmiLibrary(string lib_name) {
#ifdef WIN32
    handle_ = LoadLibrary(lib_name.c_str());
#else
    handle_ = dlopen(lib_name.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

    if (!handle_) {
        cout << getLastError() << endl;
        string msg = "Unable to load dynamic library '" + lib_name + "'!";
        throw runtime_error(msg);
    }

}

fmi2String FmiLibrary::getVersion() const {
    return loadFunction<fmi2GetVersionTYPE *>("fmi2GetVersion")();
}

fmi2String FmiLibrary::getTypesPlatform() const {
    return loadFunction<fmi2GetTypesPlatformTYPE *>("fmi2GetTypesPlatform")();
}

void FmiLibrary::instantiate(const string instanceName, const fmi2Type type, const string guid,
                             const string resourceLocation, const bool visible, const bool loggingOn) {
    c_ = loadFunction<fmi2InstantiateTYPE *>("fmi2Instantiate")(instanceName.c_str(),
            type, guid.c_str(), resourceLocation.c_str(), &callback, visible ? 1 : 0, loggingOn ? 1 : 0);

    if (c_ == nullptr) {
        throw runtime_error("Unable to instantiate FMU instance!");
    }

}

fmi2Status FmiLibrary::setupExperiment(const bool toleranceDefined, const double tolerance, const double startTime, const double stopTime) const {

    fmi2Boolean stopDefined = stopTime > startTime;
    return loadFunction<fmi2SetupExperimentTYPE *>("fmi2SetupExperiment")
            (c_, toleranceDefined ? 1 : 0, tolerance, startTime, stopDefined, stopTime);
}

fmi2Status FmiLibrary::enterInitializationMode() const {
    return loadFunction<fmi2EnterInitializationModeTYPE *>("fmi2EnterInitializationMode")(c_);
}

fmi2Status FmiLibrary::exitInitializationMode() const {
    return loadFunction<fmi2ExitInitializationModeTYPE *>("fmi2ExitInitializationMode")(c_);
}

fmi2Status FmiLibrary::reset() const {
    return loadFunction<fmi2ResetTYPE *>("fmi2Reset")(c_);
}

fmi2Status FmiLibrary::terminate() {
    return loadFunction<fmi2TerminateTYPE *>("fmi2Terminate")(c_);
}

fmi2Status FmiLibrary::readInteger(const fmi2ValueReference vr, fmi2Integer &ref) const {
    return loadFunction<fmi2GetIntegerTYPE *>("fmi2GetInteger")(c_, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readInteger(const vector<fmi2ValueReference> &vr, vector<fmi2Integer> &ref) const {
    return loadFunction<fmi2GetIntegerTYPE *>("fmi2GetInteger")(c_, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readReal(const fmi2ValueReference vr, fmi2Real &ref) const {
    return loadFunction<fmi2GetRealTYPE *>("fmi2GetReal")(c_, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readReal(const vector<fmi2ValueReference > &vr, vector<fmi2Real> &ref) const {
    return loadFunction<fmi2GetRealTYPE *>("fmi2GetReal")(c_, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readString(const fmi2ValueReference vr, fmi2String &ref) const {
    return loadFunction<fmi2GetStringTYPE *>("fmi2GetString")(c_, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readString(const vector<fmi2ValueReference> &vr, vector<fmi2String > &ref) const {
    return loadFunction<fmi2GetStringTYPE *>("fmi2GetString")(c_, vr.data(), vr.size(), ref.data());
}

fmi2Status FmiLibrary::readBoolean(const fmi2ValueReference vr, fmi2Boolean &ref) const {
    return loadFunction<fmi2GetBooleanTYPE *>("fmi2GetBoolean")(c_, &vr, 1, &ref);
}

fmi2Status FmiLibrary::readBoolean(const vector<fmi2ValueReference> &vr, vector<fmi2Boolean> &ref) const {
    return loadFunction<fmi2GetBooleanTYPE *>("fmi2GetBoolean")(c_, vr.data(), vr.size(), ref.data());
}

void FmiLibrary::freeInstance() {
    return loadFunction<fmi2FreeInstanceTYPE *>("fmi2FreeInstance")(c_);
}

template<class T>
T FmiLibrary::loadFunction(const char *function_name) const {
#ifdef WIN32
    return (T) GetProcAddress(handle_, function_name);
#else
    return (T) dlsym(handle_, function_name);
#endif
}

FmiLibrary::~FmiLibrary() {

    cout << "FmiLibrary destructor called.." << endl;

    if (c_) {
        terminate();
        freeInstance();
        c_ = nullptr;
    }

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

CoSimulationLibrary::CoSimulationLibrary(const string lib_name) : FmiLibrary(lib_name) {}

fmi2Status CoSimulationLibrary::doStep(fmi2Real currentCommunicationPoint,
        fmi2Real communicationStepSize, bool noSetFMUStatePriorToCurrentPoint) const {
    return loadFunction<fmi2DoStepTYPE *>("fmi2DoStep")(
            c_, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint ? 1 : 0);
}

fmi2Status CoSimulationLibrary::cancelStep() const {
    return loadFunction<fmi2CancelStepTYPE *>("fmi2CancelStep")(c_);
}

ModelExchangeLibrary::ModelExchangeLibrary(const string lib_name) : FmiLibrary(lib_name) {}

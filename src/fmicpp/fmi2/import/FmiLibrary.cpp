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

    static void logger(void* fmi2ComponentEnvironment, fmi2String instance_name, fmi2Status status, fmi2String category, fmi2String message, ...) {
        printf("status = %s, instanceName = %s, category = %s: %s\n", status_to_string(status), instance_name, category, message);
    }

    static const fmi2CallbackFunctions callback = {
            .logger = logger,
            .allocateMemory = calloc,
            .freeMemory = free,
            .stepFinished = NULL,
            .componentEnvironment = NULL
    };

}


FmiLibrary::FmiLibrary(string lib_name) {
#ifdef WIN32
    handle_ = LoadLibrary(lib_name.c_str());
#else
    handle_ = dlopen(lib_name.c_str(), RTLD_NOW | RTLD_LOCAL);
    cout << dlerror() << endl;
#endif

    if (!handle_) {
        string msg = "Unable to load dynamic library '" + lib_name + "'!";
        throw std::runtime_error(msg);
    }

}

fmi2String FmiLibrary::getVersion() const {
    return loadFunction<fmi2GetVersionTYPE *>("fmi2GetVersion")();
}

fmi2String FmiLibrary::getTypesPlatform() const {
    return loadFunction<fmi2GetTypesPlatformTYPE *>("fmi2GetTypesPlatform")();
}

fmi2Status FmiLibrary::setupExperiment(fmi2Component c,
        bool toleranceDefined, double tolerance, double startTime, double stopTime) const {

    fmi2Boolean stopDefined = stopTime > startTime;
    return loadFunction<fmi2SetupExperimentTYPE *>("fmi2SetupExperiment")(c, toleranceDefined ? 1 : 0, tolerance, startTime, stopDefined, stopTime);
}

fmi2Status FmiLibrary::enterInitializationMode(fmi2Component c) const {
    return loadFunction<fmi2EnterInitializationModeTYPE *>("fmi2EnterInitializationMode")(c);
}

fmi2Status FmiLibrary::exitInitializationMode(fmi2Component c) const {
    return loadFunction<fmi2ExitInitializationModeTYPE *>("fmi2ExitInitializationMode")(c);
}

fmi2Component FmiLibrary::instantiate(const string instanceName, fmi2Type type, const string guid,
                        const string resourceLocation, bool visible, bool loggingOn) {
    return loadFunction<fmi2InstantiateTYPE *>("fmi2Instantiate")(instanceName.c_str(), type, guid.c_str(),
            resourceLocation.c_str(), &callback, visible ? 1 : 0, loggingOn ? 1 : 0);
}

fmi2Status FmiLibrary::reset(fmi2Component c) const {
    return loadFunction<fmi2ResetTYPE *>("reset")(c);
}

fmi2Status FmiLibrary::terminate(fmi2Component c) const {
    return loadFunction<fmi2TerminateTYPE *>("terminate")(c);
}

fmi2Status FmiLibrary::getInteger(fmi2Component c, const vector<fmi2ValueReference> &vr, vector<fmi2Integer> values) const {
    return loadFunction<fmi2GetIntegerTYPE *>("fmi2GetInteger")(c, vr.data(), values.size(), values.data());
}

fmi2Status FmiLibrary::getReal(fmi2Component c, const vector<fmi2ValueReference > &vr, vector<fmi2Real> values) const {
    return loadFunction<fmi2GetRealTYPE *>("fmi2GetReal")(c, vr.data(), values.size(), values.data());
}

void FmiLibrary::freeInstance(fmi2Component c) const {
    return loadFunction<fmi2FreeInstanceTYPE *>("free")(c);
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

   if (handle_) {
#ifdef WIN32
       FreeLibrary(handle_)
#else
       if (dlclose(handle_) != 0) {
           cout << dlerror() << endl;
       }
#endif
       handle_ = nullptr;
   }

}

CoSimulationLibrary::CoSimulationLibrary(const string &lib_name) : FmiLibrary(lib_name) {}

fmi2Status CoSimulationLibrary::doStep(
        fmi2Component c, fmi2Real currentCommunicationPoint,
        fmi2Real communicationStepSize, bool noSetFMUStatePriorToCurrentPoint) const {
    return loadFunction<fmi2DoStepTYPE *>("fmi2DoStep")(c, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint ? 1 : 0);
}

fmi2Status CoSimulationLibrary::cancelStep(fmi2Component c) const {
    return loadFunction<fmi2CancelStepTYPE *>("fmi2CancelStep")(c);
}

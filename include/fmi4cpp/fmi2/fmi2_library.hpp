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

#ifndef FMI4CPP_FMI2LIBRARY_HPP
#define FMI4CPP_FMI2LIBRARY_HPP

#include <fmi4cpp/dll_handle.hpp>
#include <fmi4cpp/fmi2/fmi2FunctionTypes.h>
#include <fmi4cpp/fmu_resource.hpp>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace fmi4cpp::fmi2
{

class fmi2_library
{

private:
    std::shared_ptr<fmu_resource> resource_;

    fmi2GetVersionTYPE* fmi2GetVersion_;
    fmi2GetTypesPlatformTYPE* fmi2GetTypesPlatform_;

    fmi2SetDebugLoggingTYPE* fmi2SetDebugLogging_;

    fmi2InstantiateTYPE* fmi2Instantiate_;
    fmi2SetupExperimentTYPE* fmi2SetupExperiment_;
    fmi2EnterInitializationModeTYPE* fmi2EnterInitializationMode_;
    fmi2ExitInitializationModeTYPE* fmi2ExitInitializationMode_;

    fmi2ResetTYPE* fmi2Reset_;
    fmi2TerminateTYPE* fmi2Terminate_;

    fmi2GetIntegerTYPE* fmi2GetInteger_;
    fmi2GetRealTYPE* fmi2GetReal_;
    fmi2GetStringTYPE* fmi2GetString_;
    fmi2GetBooleanTYPE* fmi2GetBoolean_;

    fmi2SetIntegerTYPE* fmi2SetInteger_;
    fmi2SetRealTYPE* fmi2SetReal_;
    fmi2SetStringTYPE* fmi2SetString_;
    fmi2SetBooleanTYPE* fmi2SetBoolean_;

    fmi2GetFMUstateTYPE* fmi2GetFMUstate_;
    fmi2SetFMUstateTYPE* fmi2SetFMUstate_;
    fmi2FreeFMUstateTYPE* fmi2FreeFMUstate_;

    fmi2SerializedFMUstateSizeTYPE* fmi2SerializedFMUstateSize_;
    fmi2SerializeFMUstateTYPE* fmi2SerializeFMUstate_;
    fmi2DeSerializeFMUstateTYPE* fmi2DeSerializeFMUstate_;

    fmi2GetDirectionalDerivativeTYPE* fmi2GetDirectionalDerivative_;

    fmi2FreeInstanceTYPE* fmi2FreeInstance_;

protected:
    fmi2Status lastStatus_;
    DLL_HANDLE handle_ = nullptr;
    bool update_status_and_return_true_if_ok(fmi2Status status);

public:
    fmi2_library(const std::string& modelIdentifier, const std::shared_ptr<fmu_resource>& resource);

    [[nodiscard]] fmi2Status last_status() const;
    [[nodiscard]] fmi2String get_version() const;
    [[nodiscard]] fmi2String get_types_platform() const;

    bool set_debug_logging(fmi2Component c, bool loggingOn, std::vector<fmi2String> categories);

    fmi2Component instantiate(const std::string& instanceName, fmi2Type type,
        const std::string& guid, const std::string& resourceLocation,
        bool visible = false, bool loggingOn = false);

    bool setup_experiment(fmi2Component c, double tolerance, double startTime, double stopTime);
    bool enter_initialization_mode(fmi2Component c);
    bool exit_initialization_mode(fmi2Component c);

    bool reset(fmi2Component c);
    bool terminate(fmi2Component c);

    bool read_integer(fmi2Component c, fmi2ValueReference vr, fmi2Integer& ref);
    bool read_integer(fmi2Component c, const std::vector<fmi2ValueReference>& vr, std::vector<fmi2Integer>& ref);

    bool read_real(fmi2Component c, fmi2ValueReference vr, fmi2Real& ref);
    bool read_real(fmi2Component c, const std::vector<fmi2ValueReference>& vr, std::vector<fmi2Real>& ref);

    bool read_string(fmi2Component c, fmi2ValueReference vr, fmi2String& ref);
    bool read_string(fmi2Component c, const std::vector<fmi2ValueReference>& vr, std::vector<fmi2String>& ref);

    bool read_boolean(fmi2Component c, fmi2ValueReference vr, fmi2Boolean& ref);
    bool read_boolean(fmi2Component c, const std::vector<fmi2ValueReference>& vr, std::vector<fmi2Boolean>& ref);

    bool write_integer(fmi2Component c, fmi2ValueReference vr, const fmi2Integer& value);
    bool write_integer(fmi2Component c, const std::vector<fmi2ValueReference>& vr,
        const std::vector<fmi2Integer>& values);

    bool write_real(fmi2Component c, fmi2ValueReference vr, const fmi2Real& value);
    bool write_real(fmi2Component c, const std::vector<fmi2ValueReference>& vr, const std::vector<fmi2Real>& values);

    bool write_string(fmi2Component c, fmi2ValueReference vr, fmi2String& value);
    bool write_string(fmi2Component c, const std::vector<fmi2ValueReference>& vr,
        const std::vector<fmi2String>& values);

    bool write_boolean(fmi2Component c, fmi2ValueReference vr, const fmi2Boolean& value);
    bool write_boolean(fmi2Component c, const std::vector<fmi2ValueReference>& vr,
        const std::vector<fmi2Boolean>& values);

    bool get_fmu_state(fmi2Component c, fmi2FMUstate& state);
    bool set_fmu_state(fmi2Component c, fmi2FMUstate state);
    bool free_fmu_state(fmi2Component c, fmi2FMUstate& state);

    bool get_serialized_fmu_state_size(fmi2Component c, fmi2FMUstate state, size_t& size);
    bool serialize_fmu_state(fmi2Component c, const fmi2FMUstate& state, std::vector<fmi2Byte>& serializedState);
    bool de_serialize_fmu_state(fmi2Component c, fmi2FMUstate& state, const std::vector<fmi2Byte>& serializedState);

    bool get_directional_derivative(fmi2Component c,
        const std::vector<fmi2ValueReference>& vUnknownRef,
        const std::vector<fmi2ValueReference>& vKnownRef,
        const std::vector<fmi2Real>& dvKnownRef, std::vector<fmi2Real>& dvUnknownRef);

    void free_instance(fmi2Component c);

    ~fmi2_library();
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2LIBRARY_HPP

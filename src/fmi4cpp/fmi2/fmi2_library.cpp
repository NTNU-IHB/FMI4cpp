
#include <fmi4cpp/fmi2/fmi2_library.hpp>
#include <fmi4cpp/library_helper.hpp>
#include <fmi4cpp/mlog.hpp>
#include <fmi4cpp/tools/os_util.hpp>

#include <cstdarg>
#include <sstream>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

namespace
{

std::string to_string(fmi2Status status)
{
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

void logger(void* fmi2ComponentEnvironment, fmi2String instance_name, fmi2Status status, fmi2String category,
    fmi2String message, ...)
{
    char msg[1000];
    va_list argp;

    va_start(argp, message);
    vsprintf(msg, message, argp);
    va_end(argp);

    MLOG_INFO("[FMI callback logger] status=" + to_string(status) + ", instanceName=" + instance_name +
        ", category=" + category + ", message=" + msg);
}

const fmi2CallbackFunctions callback = {
    logger, calloc, free, nullptr, nullptr};

} // namespace


fmi2_library::fmi2_library(const std::string& modelIdentifier, const std::shared_ptr<fmu_resource>& resource)
    : resource_(resource)
{
    const std::string libName = resource->absolute_library_path(modelIdentifier);

    MLOG_DEBUG("Loading shared library '" + std::filesystem::path(libName).stem().string() + get_shared_library_extension() + "'");

#ifdef _WIN32
    std::string dllDirectory;
    fmi4cpp::fs::path path(libName);

    if (path.has_parent_path()) {
        dllDirectory = path.parent_path().string();
    }

    if (!dllDirectory.empty()) {
        std::wstring wDllDirectory(dllDirectory.begin(), dllDirectory.end());
        dllDirectoryCookie_ = AddDllDirectory(wDllDirectory.c_str());
    }
#endif

    handle_ = load_library(libName);

    if (!handle_) {
        const auto err = "Unable to load dynamic library '" + libName + "'! " + getLastError();
        MLOG_ERROR(err);
        throw std::runtime_error(err);
    }

    fmi2GetVersion_ = load_function<fmi2GetVersionTYPE*>(handle_, "fmi2GetVersion");
    fmi2GetTypesPlatform_ = load_function<fmi2GetTypesPlatformTYPE*>(handle_, "fmi2GetTypesPlatform");

    fmi2SetDebugLogging_ = load_function<fmi2SetDebugLoggingTYPE*>(handle_, "fmi2SetDebugLogging");

    fmi2Instantiate_ = load_function<fmi2InstantiateTYPE*>(handle_, "fmi2Instantiate");
    fmi2SetupExperiment_ = load_function<fmi2SetupExperimentTYPE*>(handle_, "fmi2SetupExperiment");
    fmi2EnterInitializationMode_ = load_function<fmi2EnterInitializationModeTYPE*>(handle_,
        "fmi2EnterInitializationMode");
    fmi2ExitInitializationMode_ = load_function<fmi2ExitInitializationModeTYPE*>(handle_,
        "fmi2ExitInitializationMode");

    fmi2Reset_ = load_function<fmi2ResetTYPE*>(handle_, "fmi2Reset");
    fmi2Terminate_ = load_function<fmi2TerminateTYPE*>(handle_, "fmi2Terminate");

    fmi2GetInteger_ = load_function<fmi2GetIntegerTYPE*>(handle_, "fmi2GetInteger");
    fmi2GetReal_ = load_function<fmi2GetRealTYPE*>(handle_, "fmi2GetReal");
    fmi2GetString_ = load_function<fmi2GetStringTYPE*>(handle_, "fmi2GetString");
    fmi2GetBoolean_ = load_function<fmi2GetBooleanTYPE*>(handle_, "fmi2GetBoolean");

    fmi2SetInteger_ = load_function<fmi2SetIntegerTYPE*>(handle_, "fmi2SetInteger");
    fmi2SetReal_ = load_function<fmi2SetRealTYPE*>(handle_, "fmi2SetReal");
    fmi2SetString_ = load_function<fmi2SetStringTYPE*>(handle_, "fmi2SetString");
    fmi2SetBoolean_ = load_function<fmi2SetBooleanTYPE*>(handle_, "fmi2SetBoolean");

    fmi2GetFMUstate_ = load_function<fmi2GetFMUstateTYPE*>(handle_, "fmi2GetFMUstate");
    fmi2SetFMUstate_ = load_function<fmi2SetFMUstateTYPE*>(handle_, "fmi2SetFMUstate");
    fmi2FreeFMUstate_ = load_function<fmi2FreeFMUstateTYPE*>(handle_, "fmi2FreeFMUstate");
    fmi2SerializeFMUstate_ = load_function<fmi2SerializeFMUstateTYPE*>(handle_, "fmi2SerializeFMUstate");
    fmi2DeSerializeFMUstate_ = load_function<fmi2DeSerializeFMUstateTYPE*>(handle_, "fmi2DeSerializeFMUstate");

    fmi2GetDirectionalDerivative_ = load_function<fmi2GetDirectionalDerivativeTYPE*>(handle_,
        "fmi2GetDirectionalDerivative");

    fmi2FreeInstance_ = load_function<fmi2FreeInstanceTYPE*>(handle_, "fmi2FreeInstance");
}

bool fmi2_library::update_status_and_return_true_if_ok(fmi2Status status)
{
    lastStatus_ = status;
    return status == fmi2OK;
}

DLL_HANDLE fmi2_library::handle() const
{
    return handle_;
}

fmi2Status fmi2_library::last_status() const
{
    return lastStatus_;
}

fmi2String fmi2_library::get_version() const
{
    return fmi2GetVersion_();
}

fmi2String fmi2_library::get_types_platform() const
{
    return fmi2GetTypesPlatform_();
}

fmi2Component fmi2_library::instantiate(const std::string& instanceName, const fmi2Type type,
    const std::string& guid, const std::string& resourceLocation,
    bool visible, bool loggingOn)
{
    fmi2Component c = fmi2Instantiate_(instanceName.c_str(), type, guid.c_str(),
        resourceLocation.c_str(), &callback, visible, loggingOn);

    if (c == nullptr) {
        const std::string msg = "Fatal: fmi2Instantiate returned nullptr, unable to instantiate FMU instance!";
        MLOG_ERROR(msg);
        throw std::runtime_error(msg);
    }

    return c;
}

bool fmi2_library::set_debug_logging(fmi2Component c, bool loggingOn,
    std::vector<fmi2String> categories)
{
    return update_status_and_return_true_if_ok(
        fmi2SetDebugLogging_(c, loggingOn, categories.size(), categories.data()));
}

bool fmi2_library::setup_experiment(fmi2Component c, double tolerance, double startTime, double stopTime)
{
    bool stopDefined = (stopTime > startTime);
    bool toleranceDefined = (tolerance > 0);
    MLOG_INFO("Calling fmi2SetupExperiment with toleranceDefined=" +
            std::string((toleranceDefined ? "true" : "false"))
        << ", tolerance=" << tolerance
        << ", startTime=" << startTime
        << ", stopTimeDefined=" << std::string((stopDefined ? "true" : "false")) << ", stop=" << stopTime)
    return update_status_and_return_true_if_ok(
        fmi2SetupExperiment_(c, toleranceDefined, tolerance, startTime, stopDefined, stopTime));
}

bool fmi2_library::enter_initialization_mode(fmi2Component c)
{
    return update_status_and_return_true_if_ok(
        fmi2EnterInitializationMode_(c));
}

bool fmi2_library::exit_initialization_mode(fmi2Component c)
{
    return update_status_and_return_true_if_ok(
        fmi2ExitInitializationMode_(c));
}

bool fmi2_library::reset(fmi2Component c)
{
    return update_status_and_return_true_if_ok(
        fmi2Reset_(c));
}

bool fmi2_library::terminate(fmi2Component c)
{
    return update_status_and_return_true_if_ok(
        fmi2Terminate_(c));
}

bool fmi2_library::read_integer(
    fmi2Component c,
    fmi2ValueReference vr,
    fmi2Integer& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetInteger_(c, &vr, 1, &ref));
}

bool fmi2_library::read_integer(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    std::vector<fmi2Integer>& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetInteger_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2_library::read_real(
    fmi2Component c,
    fmi2ValueReference vr,
    fmi2Real& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetReal_(c, &vr, 1, &ref));
}

bool fmi2_library::read_real(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    std::vector<fmi2Real>& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetReal_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2_library::read_string(
    fmi2Component c,
    fmi2ValueReference vr,
    fmi2String& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetString_(c, &vr, 1, &ref));
}

bool fmi2_library::read_string(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    std::vector<fmi2String>& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetString_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2_library::read_boolean(
    fmi2Component c,
    fmi2ValueReference vr,
    fmi2Boolean& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetBoolean_(c, &vr, 1, &ref));
}

bool fmi2_library::read_boolean(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    std::vector<fmi2Boolean>& ref)
{
    return update_status_and_return_true_if_ok(
        fmi2GetBoolean_(c, vr.data(), vr.size(), ref.data()));
}

bool fmi2_library::write_integer(
    fmi2Component c,
    fmi2ValueReference vr,
    const fmi2Integer& value)
{
    return update_status_and_return_true_if_ok(
        fmi2SetInteger_(c, &vr, 1, &value));
}

bool fmi2_library::write_integer(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    const std::vector<fmi2Integer>& values)
{
    return update_status_and_return_true_if_ok(
        fmi2SetInteger_(c, vr.data(), vr.size(), values.data()));
}

bool fmi2_library::write_real(
    fmi2Component c,
    fmi2ValueReference vr,
    const fmi2Real& value)
{
    return update_status_and_return_true_if_ok(
        fmi2SetReal_(c, &vr, 1, &value));
}

bool fmi2_library::write_real(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    const std::vector<fmi2Real>& values)
{
    return update_status_and_return_true_if_ok(
        fmi2SetReal_(c, vr.data(), vr.size(), values.data()));
}

bool fmi2_library::write_string(
    fmi2Component c,
    fmi2ValueReference vr,
    fmi2String& value)
{
    return update_status_and_return_true_if_ok(
        fmi2SetString_(c, &vr, 1, &value));
}

bool fmi2_library::write_string(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    const std::vector<fmi2String>& values)
{
    return update_status_and_return_true_if_ok(
        fmi2SetString_(c, vr.data(), vr.size(), values.data()));
}

bool fmi2_library::write_boolean(
    fmi2Component c,
    fmi2ValueReference vr,
    const fmi2Boolean& value)
{
    return update_status_and_return_true_if_ok(
        fmi2SetBoolean_(c, &vr, 1, &value));
}

bool fmi2_library::write_boolean(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    const std::vector<fmi2Boolean>& values)
{
    return update_status_and_return_true_if_ok(
        fmi2SetBoolean_(c,
            vr.data(),
            vr.size(),
            values.data()));
}

bool fmi2_library::get_fmu_state(
    fmi2Component c,
    fmi2FMUstate& state)
{
    return update_status_and_return_true_if_ok(
        fmi2GetFMUstate_(c, &state));
}

bool fmi2_library::set_fmu_state(
    fmi2Component c,
    fmi2FMUstate state)
{
    return update_status_and_return_true_if_ok(
        fmi2SetFMUstate_(c, state));
}

bool fmi2_library::free_fmu_state(
    fmi2Component c,
    fmi2FMUstate& state)
{
    return update_status_and_return_true_if_ok(
        fmi2FreeFMUstate_(c, &state));
}

bool fmi4cpp::fmi2::fmi2_library::get_serialized_fmu_state_size(
    fmi2Component c,
    fmi2FMUstate state,
    size_t& size)
{
    return update_status_and_return_true_if_ok(
        fmi2SerializedFMUstateSize_(c, state, &size));
}

bool fmi2_library::serialize_fmu_state(
    fmi2Component c,
    const fmi2FMUstate& state,
    std::vector<fmi2Byte>& serializedState)
{
    size_t size = 0;
    get_serialized_fmu_state_size(c, state, size);
    serializedState.reserve(size);
    return update_status_and_return_true_if_ok(
        fmi2SerializeFMUstate_(c,
            state,
            serializedState.data(),
            size));
}

bool fmi2_library::de_serialize_fmu_state(
    fmi2Component c,
    fmi2FMUstate& state,
    const std::vector<fmi2Byte>& serializedState)
{
    return update_status_and_return_true_if_ok(
        fmi2DeSerializeFMUstate_(c,
            serializedState.data(),
            serializedState.size(),
            &state));
}

bool fmi2_library::get_directional_derivative(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vUnknownRef,
    const std::vector<fmi2ValueReference>& vKnownRef,
    const std::vector<fmi2Real>& dvKnownRef,
    std::vector<fmi2Real>& dvUnknownRef)
{
    return update_status_and_return_true_if_ok(
        fmi2GetDirectionalDerivative_(c,
            vUnknownRef.data(), vUnknownRef.size(),
            vKnownRef.data(), vKnownRef.size(),
            dvKnownRef.data(), dvUnknownRef.data()));
}

void fmi2_library::free_instance(fmi2Component c)
{
    fmi2FreeInstance_(c);
}

fmi2_library::~fmi2_library()
{
#ifdef _WIN32
    if (dllDirectoryCookie_) {
        RemoveDllDirectory(dllDirectoryCookie_);
        dllDirectoryCookie_ = nullptr;
    }
#endif

    if (handle_) {
        if (!free_library(handle_)) {
            MLOG_ERROR(getLastError());
        }
        handle_ = nullptr;
    }
}

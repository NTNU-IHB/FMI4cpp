
#ifndef FMI4CPP_FMUINSTANCE_HPP
#define FMI4CPP_FMUINSTANCE_HPP

#include <fmi4cpp/fmu_variable_accessor.hpp>
#include <fmi4cpp/dll_handle.hpp>
#include <fmi4cpp/status.hpp>
#include <fmi4cpp/types.hpp>

#include <memory>
#include <vector>

namespace fmi4cpp
{

template<typename ModelDescription>
class fmu_instance : public fmu_variable_accessor
{

protected:
    double simulationTime_ = 0.0;

public:
    [[nodiscard]] virtual double get_simulation_time() const
    {
        return simulationTime_;
    }

    [[nodiscard]] virtual DLL_HANDLE handle() const = 0;

    [[nodiscard]] virtual status last_status() const = 0;

    virtual std::shared_ptr<const ModelDescription> get_model_description() const = 0;

    virtual bool setup_experiment(double startTime = 0.0, double stopTime = 0.0, double tolerance = 0.0) = 0;
    virtual bool enter_initialization_mode() = 0;
    virtual bool exit_initialization_mode() = 0;

    virtual bool reset() = 0;
    virtual bool terminate() = 0;

    virtual bool get_fmu_state(fmi4cppFMUstate& state) = 0;
    virtual bool set_fmu_state(fmi4cppFMUstate state) = 0;
    virtual bool free_fmu_state(fmi4cppFMUstate& state) = 0;

    virtual bool serialize_fmu_state(
        const fmi4cppFMUstate& state,
        std::vector<fmi4cppByte>& serializedState) = 0;
    virtual bool de_serialize_fmu_state(
        fmi4cppFMUstate& state,
        const std::vector<fmi4cppByte>& serializedState) = 0;

    virtual bool get_directional_derivative(
        const std::vector<fmi4cppValueReference>& vUnknownRef,
        const std::vector<fmi4cppValueReference>& vKnownRef,
        const std::vector<fmi4cppReal>& dvKnownRef,
        std::vector<fmi4cppReal>& dvUnknownRef) = 0;

    ~fmu_instance() override = default;
};

} // namespace fmi4cpp

#endif //FMI4CPP_FMUINSTANCE_HPP

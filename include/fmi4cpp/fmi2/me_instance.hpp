
#ifndef FMI4CPP_FMI2MODELEXCHANGEINSTANCE_HPP
#define FMI4CPP_FMI2MODELEXCHANGEINSTANCE_HPP

#include <fmi4cpp/fmi2/me_library.hpp>
#include <fmi4cpp/fmi2/xml/me_model_description.hpp>
#include <fmi4cpp/fmu_instance_base.hpp>
#include <fmi4cpp/fmu_resource.hpp>

#include <memory>
#include <vector>

namespace fmi4cpp::fmi2
{

class me_instance : public fmu_instance_base<me_library, me_model_description>
{

public:
    fmi2EventInfo eventInfo_;

    me_instance(fmi2Component c,
        const std::shared_ptr<fmu_resource>& resource,
        const std::shared_ptr<me_library>& library,
        const std::shared_ptr<const me_model_description>& modelDescription);


    bool enter_event_mode();

    bool enter_continuous_time_mode();

    bool set_time(double time);

    bool set_continuous_states(const std::vector<fmi2Real>& x);

    bool get_derivatives(std::vector<fmi2Real>& derivatives);

    bool get_event_indicators(std::vector<fmi2Real>& eventIndicators);

    bool get_continuous_states(std::vector<fmi2Real>& x);

    bool get_nominals_of_continuous_states(std::vector<fmi2Real>& x_nominal);

    bool completed_integrator_step(fmi2Boolean noSetFMUStatePriorToCurrentPoint,
        fmi2Boolean& enterEventMode, fmi2Boolean& terminateSimulation);

    bool new_discrete_states();

    [[nodiscard]] DLL_HANDLE handle() const override;

    [[nodiscard]] fmi4cpp::status last_status() const override;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2MODELEXCHANGEINSTANCE_HPP


#ifndef FMI4CPP_FMI2MODELEXCHANGELIBRARY_HPP
#define FMI4CPP_FMI2MODELEXCHANGELIBRARY_HPP

#include <fmi4cpp/fmi2/fmi2_library.hpp>

namespace fmi4cpp::fmi2
{

class me_library : public fmi2_library
{

private:
    fmi2EnterEventModeTYPE* fmi2EnterEventMode_;
    fmi2EnterContinuousTimeModeTYPE* fmi2EnterContinuousTimeMode_;
    fmi2SetTimeTYPE* fmi2SetTime_;
    fmi2SetContinuousStatesTYPE* fmi2SetContinuousStates_;
    fmi2GetDerivativesTYPE* fmi2GetDerivatives_;
    fmi2GetEventIndicatorsTYPE* fmi2GetEventIndicators_;
    fmi2GetContinuousStatesTYPE* fmi2GetContinuousStates_;
    fmi2GetNominalsOfContinuousStatesTYPE* fmi2GetNominalsOfContinuousStates_;
    fmi2CompletedIntegratorStepTYPE* fmi2CompletedIntegratorStep_;
    fmi2NewDiscreteStatesTYPE* fmi2NewDiscreteStates_;

public:
    explicit me_library(
        const std::string& modelIdentifier,
        const std::shared_ptr<fmu_resource>& resource);

    bool enter_event_mode(fmi2Component c);

    bool enter_continuous_time_mode(fmi2Component c);

    bool set_time(fmi2Component c, double time);

    bool set_continuous_states(fmi2Component c, const std::vector<fmi2Real>& x);

    bool get_derivatives(fmi2Component c, std::vector<fmi2Real>& derivatives);

    bool get_event_indicators(fmi2Component c, std::vector<fmi2Real>& eventIndicators);

    bool get_continuous_states(fmi2Component c, std::vector<fmi2Real>& x);

    bool get_nominals_of_continuous_states(fmi2Component c, std::vector<fmi2Real>& x_nominal);

    bool completed_integrator_step(fmi2Component c,
        fmi2Boolean noSetFMUStatePriorToCurrentPoint,
        fmi2Boolean& enterEventMode,
        fmi2Boolean& terminateSimulation);

    bool new_discrete_states(fmi2Component c, fmi2EventInfo& eventInfo);
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2MODELEXCHANGELIBRARY_HPP

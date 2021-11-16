
#include <fmi4cpp/fmi2/me_library.hpp>
#include <fmi4cpp/library_helper.hpp>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

me_library::me_library(const std::string& modelIdentifier,
    const std::shared_ptr<fmu_resource>& resource)
    : fmi2_library(modelIdentifier, resource)
{

    fmi2EnterEventMode_ = load_function<fmi2EnterEventModeTYPE*>(handle_, "fmi2EnterEventMode");
    fmi2EnterContinuousTimeMode_ = load_function<fmi2EnterContinuousTimeModeTYPE*>(
        handle_, "fmi2EnterContinuousTimeMode");
    fmi2SetTime_ = load_function<fmi2SetTimeTYPE*>(handle_, "fmi2SetTime");
    fmi2SetContinuousStates_ = load_function<fmi2SetContinuousStatesTYPE*>(handle_, "fmi2SetContinuousStates");
    fmi2GetDerivatives_ = load_function<fmi2GetDerivativesTYPE*>(handle_, "fmi2GetDerivatives");
    fmi2GetEventIndicators_ = load_function<fmi2GetEventIndicatorsTYPE*>(handle_, "fmi2GetEventIndicators");
    fmi2GetContinuousStates_ = load_function<fmi2GetContinuousStatesTYPE*>(handle_, "fmi2GetContinuousStates");
    fmi2GetNominalsOfContinuousStates_ = load_function<fmi2GetNominalsOfContinuousStatesTYPE*>(
        handle_, "fmi2GetNominalsOfContinuousStates");
    fmi2CompletedIntegratorStep_ = load_function<fmi2CompletedIntegratorStepTYPE*>(handle_,
        "fmi2CompletedIntegratorStep");
    fmi2NewDiscreteStates_ = load_function<fmi2NewDiscreteStatesTYPE*>(handle_, "fmi2NewDiscreteStates");
}

bool me_library::enter_event_mode(fmi2Component c)
{
    return update_status_and_return_true_if_ok(
        fmi2EnterEventMode_(c));
}

bool me_library::enter_continuous_time_mode(fmi2Component c)
{
    return update_status_and_return_true_if_ok(
        fmi2EnterContinuousTimeMode_(c));
}

bool me_library::set_time(
    fmi2Component c,
    double time)
{
    return update_status_and_return_true_if_ok(
        fmi2SetTime_(c, time));
}

bool me_library::set_continuous_states(
    fmi2Component c,
    const std::vector<fmi2Real>& x)
{
    return update_status_and_return_true_if_ok(
        fmi2SetContinuousStates_(c, x.data(), x.size()));
}

bool me_library::get_derivatives(
    fmi2Component c,
    std::vector<fmi2Real>& derivatives)
{
    return update_status_and_return_true_if_ok(
        fmi2GetDerivatives_(c, derivatives.data(), derivatives.size()));
}

bool me_library::get_event_indicators(
    fmi2Component c,
    std::vector<fmi2Real>& eventIndicators)
{
    return update_status_and_return_true_if_ok(
        fmi2GetEventIndicators_(c, eventIndicators.data(), eventIndicators.size()));
}

bool me_library::get_continuous_states(
    fmi2Component c,
    std::vector<fmi2Real>& x)
{
    return update_status_and_return_true_if_ok(
        fmi2GetContinuousStates_(c, x.data(), x.size()));
}

bool me_library::get_nominals_of_continuous_states(
    fmi2Component c,
    std::vector<fmi2Real>& x_nominal)
{
    return update_status_and_return_true_if_ok(
        fmi2GetNominalsOfContinuousStates_(c, x_nominal.data(), x_nominal.size()));
}

bool me_library::completed_integrator_step(
    fmi2Component c,
    fmi2Boolean noSetFMUStatePriorToCurrentPoint,
    fmi2Boolean& enterEventMode,
    fmi2Boolean& terminateSimulation)
{
    return update_status_and_return_true_if_ok(
        fmi2CompletedIntegratorStep_(c, noSetFMUStatePriorToCurrentPoint, &enterEventMode, &terminateSimulation));
}

bool me_library::new_discrete_states(
    fmi2Component c,
    fmi2EventInfo& eventInfo)
{
    return update_status_and_return_true_if_ok(
        fmi2NewDiscreteStates_(c, &eventInfo));
}

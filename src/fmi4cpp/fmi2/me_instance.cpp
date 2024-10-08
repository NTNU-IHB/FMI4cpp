
#include <fmi4cpp/fmi2/me_instance.hpp>
#include <fmi4cpp/fmi2/status_converter.hpp>

using namespace fmi4cpp::fmi2;

me_instance::me_instance(fmi2Component c,
    const std::shared_ptr<fmu_resource>& resource,
    const std::shared_ptr<me_library>& library,
    const std::shared_ptr<const me_model_description>& modelDescription)
    : fmu_instance_base<me_library, me_model_description>(c, resource, library, modelDescription)
{}

DLL_HANDLE me_instance::handle() const
{
    return library_->handle();
}

fmi4cpp::status me_instance::last_status() const
{
    return convert(library_->last_status());
}

bool me_instance::enter_event_mode()
{
    return library_->enter_event_mode(c_);
}

bool me_instance::enter_continuous_time_mode()
{
    return library_->enter_continuous_time_mode(c_);
}

bool me_instance::set_time(const double time)
{
    this->simulationTime_ = time;
    return library_->set_time(c_, time);
}

bool me_instance::set_continuous_states(const std::vector<fmi2Real>& x)
{
    return library_->set_continuous_states(c_, x);
}

bool me_instance::get_derivatives(std::vector<fmi2Real>& derivatives)
{
    return library_->get_derivatives(c_, derivatives);
}

bool me_instance::get_event_indicators(std::vector<fmi2Real>& eventIndicators)
{
    return library_->get_event_indicators(c_, eventIndicators);
}

bool me_instance::get_continuous_states(std::vector<fmi2Real>& x)
{
    return library_->get_continuous_states(c_, x);
}

bool me_instance::get_nominals_of_continuous_states(std::vector<fmi2Real>& x_nominal)
{
    return library_->get_nominals_of_continuous_states(c_, x_nominal);
}

bool me_instance::completed_integrator_step(
    fmi2Boolean noSetFMUStatePriorToCurrentPoint,
    fmi2Boolean& enterEventMode,
    fmi2Boolean& terminateSimulation)
{
    return library_->completed_integrator_step(c_,
        noSetFMUStatePriorToCurrentPoint,
        enterEventMode,
        terminateSimulation);
}

bool me_instance::new_discrete_states()
{
    return library_->new_discrete_states(c_, eventInfo_);
}

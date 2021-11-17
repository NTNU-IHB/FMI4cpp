
#include <fmi4cpp/fmi2/cs_slave.hpp>
#include <fmi4cpp/fmi2/status_converter.hpp>

using namespace fmi4cpp::fmi2;

cs_slave::cs_slave(fmi2Component c,
    const std::shared_ptr<fmi4cpp::fmu_resource>& resource,
    const std::shared_ptr<cs_library>& library,
    const std::shared_ptr<const cs_model_description>& modelDescription)
    : fmu_instance_base<cs_library, cs_model_description>(c, resource, library, modelDescription)
{}

DLL_HANDLE cs_slave::handle() const
{
    return library_->handle();
}

fmi4cpp::status cs_slave::last_status() const
{
    return convert(library_->last_status());
}

bool cs_slave::step(const double stepSize)
{
    if (library_->step(c_, simulationTime_, stepSize, false)) {
        simulationTime_ += stepSize;
        return true;
    }
    return false;
}

bool cs_slave::cancel_step()
{
    return library_->cancel_step(c_);
}

std::shared_ptr<const cs_model_description> cs_slave::get_model_description() const
{
    return fmu_instance_base::get_model_description();
}

bool cs_slave::setup_experiment(double start, double stop, double tolerance)
{
    return fmu_instance_base::setup_experiment(start, stop, tolerance);
}

bool cs_slave::enter_initialization_mode()
{
    return fmu_instance_base::enter_initialization_mode();
}

bool cs_slave::exit_initialization_mode()
{
    return fmu_instance_base::exit_initialization_mode();
}

bool cs_slave::reset()
{
    return fmu_instance_base::reset();
}

bool cs_slave::terminate()
{
    return fmu_instance_base::terminate();
}

bool cs_slave::read_integer(unsigned int vr, int& ref)
{
    return fmu_instance_base::read_integer(vr, ref);
}

bool cs_slave::read_integer(
    const std::vector<unsigned int>& vr,
    std::vector<int>& ref)
{
    return fmu_instance_base::read_integer(vr, ref);
}

bool cs_slave::read_real(unsigned int vr, double& ref)
{
    return fmu_instance_base::read_real(vr, ref);
}

bool cs_slave::read_real(
    const std::vector<unsigned int>& vr,
    std::vector<double>& ref)
{
    return fmu_instance_base::read_real(vr, ref);
}

bool cs_slave::read_string(unsigned int vr, const char*& ref)
{
    return fmu_instance_base::read_string(vr, ref);
}

bool cs_slave::read_string(
    const std::vector<unsigned int>& vr,
    std::vector<const char*>& ref)
{
    return fmu_instance_base::read_string(vr, ref);
}

bool cs_slave::read_boolean(unsigned int vr, int& ref)
{
    return fmu_instance_base::read_boolean(vr, ref);
}

bool cs_slave::read_boolean(
    const std::vector<unsigned int>& vr,
    std::vector<int>& ref)
{
    return fmu_instance_base::read_boolean(vr, ref);
}

bool cs_slave::write_integer(unsigned int vr, int value)
{
    return fmu_instance_base::write_integer(vr, value);
}

bool cs_slave::write_integer(
    const std::vector<unsigned int>& vr,
    const std::vector<int>& values)
{
    return fmu_instance_base::write_integer(vr, values);
}

bool cs_slave::write_real(unsigned int vr, double value)
{
    return fmu_instance_base::write_real(vr, value);
}

bool cs_slave::write_real(
    const std::vector<unsigned int>& vr,
    const std::vector<double>& values)
{
    return fmu_instance_base::write_real(vr, values);
}

bool cs_slave::write_string(unsigned int vr, const char* value)
{
    return fmu_instance_base::write_string(vr, value);
}

bool cs_slave::write_string(
    const std::vector<unsigned int>& vr,
    const std::vector<const char*>& values)
{
    return fmu_instance_base::write_string(vr, values);
}

bool cs_slave::write_boolean(unsigned int vr, int value)
{
    return fmu_instance_base::write_boolean(vr, value);
}

bool cs_slave::write_boolean(
    const std::vector<unsigned int>& vr,
    const std::vector<int>& values)
{
    return fmu_instance_base::write_boolean(vr, values);
}


bool cs_slave::get_fmu_state(void*& state)
{
    return fmu_instance_base::get_fmu_state(state);
}

bool cs_slave::set_fmu_state(void* state)
{
    return fmu_instance_base::set_fmu_state(state);
}

bool cs_slave::free_fmu_state(void*& state)
{
    return fmu_instance_base::free_fmu_state(state);
}

bool cs_slave::serialize_fmu_state(
    const fmi2FMUstate& state,
    std::vector<char>& serializedState)
{
    return fmu_instance_base::serialize_fmu_state(
        state, serializedState);
}

bool cs_slave::de_serialize_fmu_state(
    fmi2FMUstate& state,
    const std::vector<char>& serializedState)
{
    return fmu_instance_base::de_serialize_fmu_state(
        state, serializedState);
}

bool cs_slave::get_directional_derivative(
    const std::vector<unsigned int>& vUnknownRef,
    const std::vector<unsigned int>& vKnownRef,
    const std::vector<double>& dvKnownRef,
    std::vector<double>& dvUnknownRef)
{
    return fmu_instance_base::get_directional_derivative(
        vUnknownRef, vKnownRef, dvKnownRef, dvUnknownRef);
}

#include <fmi4cpp/fmi4cpp.hpp>

#include <iostream>
#include <string>

using namespace fmi4cpp;

const double stop = 0.01;
const double stepSize = 1E-3;

int main()
{
    const std::string fmuPath = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                                "ControlledTemperature/ControlledTemperature.fmu";

    fmi2::fmu fmu(fmuPath);
    auto cs_fmu = fmu.as_cs_fmu();
    auto md = cs_fmu->get_model_description();

    auto var = md->model_variables->getByValueReference(47).as_real();
    std::cout << "Name=" << var.name() << ", start=" << var.start().value_or(0) << std::endl;

    auto slave1 = cs_fmu->new_instance();
    auto slave2 = cs_fmu->new_instance();

    std::cout << "model_identifier=" << slave1->get_model_description()->model_identifier << std::endl;

    slave1->setup_experiment();
    slave1->enter_initialization_mode();
    slave1->exit_initialization_mode();

    slave2->setup_experiment();
    slave2->enter_initialization_mode();
    slave2->exit_initialization_mode();

    std::vector<fmi2Real> ref(2);
    std::vector<fmi2ValueReference> vr = {md->get_variable_by_name("Temperature_Reference").value_reference,
        md->get_variable_by_name("Temperature_Room").value_reference};

    double t;
    while ((t = slave1->get_simulation_time()) <= stop) {

        if (!slave1->step(stepSize)) { break; }
        if (!slave1->read_real(vr, ref)) { break; }
        std::cout << "t=" << t << ", Temperature_Reference=" << ref[0] << ", Temperature_Room=" << ref[1] << std::endl;
    }

    std::cout << "FMU '" << fmu.model_name() << "' terminated with success: " << std::boolalpha << slave1->terminate() << std::endl;
    std::cout << "FMU '" << fmu.model_name() << "' terminated with success: " << std::boolalpha << slave2->terminate() << std::endl;

    return 0;
}
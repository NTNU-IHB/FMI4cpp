#include "time_util.hpp"

#include <fmi4cpp/fmi4cpp.hpp>

#include <iostream>

using namespace fmi4cpp;

const double stop = 10.0;
const double step_size = 1E-4;

void run(std::unique_ptr<fmi2::cs_slave>& slave)
{
    slave->setup_experiment();
    slave->enter_initialization_mode();
    slave->exit_initialization_mode();

    auto elapsed = measure_time_sec([&slave] {
        double ref;
        const auto vr = slave->get_model_description()->get_variable_by_name("Temperature_Room").as_real().valueReference();
        while ((slave->get_simulation_time()) <= (stop - step_size)) {
            if (!slave->step(step_size)) {
                std::cerr << "Error! step returned with status: " << to_string(slave->last_status()) << std::endl;
                break;
            }
            if (!slave->read_real(vr, ref)) {
                std::cerr << "Error! read_real returned with status: " << to_string(slave->last_status()) << std::endl;
                break;
            }
        }
    });

    std::cout << "Time elapsed=" << elapsed << "s" << std::endl;

    slave->terminate();
}

int main()
{
    const std::string fmu_path = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                                 "ControlledTemperature/ControlledTemperature.fmu";

    auto fmu = fmi2::fmu(fmu_path).as_cs_fmu()->new_instance();
    run(fmu);

    return 0;
}
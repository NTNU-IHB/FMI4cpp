#include <fmi4cpp/fmi4cpp.hpp>

#include <iostream>
#include "time_util.hpp"

using namespace fmi4cpp;

const double stop = 12.0;
const double step_size = 1E-5;
const fmi2ValueReference vr = 2;

int main()
{
    const std::string fmu_path = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                                 "TorsionBar/TorsionBar.fmu";

    fmi2::fmu fmu(fmu_path);

    const auto slave = fmu.as_cs_fmu()->new_instance();
    slave->setup_experiment();
    slave->enter_initialization_mode();
    slave->exit_initialization_mode();

    auto elapsed = measure_time_sec([&slave] {
        double ref;
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

    return 0;
}
#include <fmi4cpp/fmi4cpp.hpp>

#include <cassert>
#include <iostream>

using namespace fmi4cpp;

int main()
{
    const std::string fmu_path1 = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                             "TorsionBar/TorsionBar.fmu";

    const std::string fmu_path2 = "../resources/fmus/2.0/cs/20sim/4.6.4.8004/"
                             "ControlledTemperature/ControlledTemperature.fmu";

    fmi2::fmu fmu1(fmu_path1);
    fmi2::fmu fmu2(fmu_path2);

    const auto slave1 = fmu1.as_cs_fmu()->new_instance();
    const auto md1 = slave1->get_model_description();

    slave1->setup_experiment();
    slave1->enter_initialization_mode();
    slave1->exit_initialization_mode();

    const auto slave2 = fmu2.as_cs_fmu()->new_instance();
    const auto md2 = slave2->get_model_description();
    slave2->setup_experiment();
    slave2->enter_initialization_mode();
    slave2->exit_initialization_mode();

    slave1->step(1E-5);
    slave2->step(1E-4);

    double ref;
    auto var = md1->get_variable_by_name("MotorDiskRev").as_real();
    assert(var.valueReference() == 105);
    var.read(*slave1, ref);
    std::cout << "MotorDiskRev=" << ref << std::endl;

    auto vr = md2->get_value_reference("Temperature_Room");
    assert(vr == 47);
    slave2->read_real(vr, ref);
    std::cout << "Temperature_Room=" << ref << std::endl;

    slave1->terminate();
    slave2->terminate();

    return 0;
}
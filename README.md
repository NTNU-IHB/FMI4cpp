# FMI4cpp (work in progress)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/NTNU-IHB/FMU-proxy/issues)
[![Join the chat at https://gitter.im/NTNU-IHB/FMI4cpp](https://badges.gitter.im/NTNU-IHB/FMI4cpp.svg)](https://gitter.im/NTNU-IHB/FMI4cpp?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

![GitHub Actions](https://github.com/NTNU-IHB/FMI4cpp/.github/workflows/config.yml/badge.svg)
[![CircleCI](https://circleci.com/gh/NTNU-IHB/FMI4cpp/tree/master.svg?style=svg)](https://circleci.com/gh/NTNU-IHB/FMI4cpp/tree/master)

FMI4cpp is a cross-platform [FMI](https://fmi-standard.org/) 2.0 implementation written in modern C++.

Influenced by it's spiritual brother [FMI4j](https://github.com/NTNU-IHB/FMI4j), it aims to be
an easy to install, easy to use, object oriented and fast FMI implementation for C++.    

FMI4cpp supports both **Co-simulation** and **Model Exchange**. <br/>

### Why should I use this over other C/C++ FMI Libraries

Because it provides a clean, easy to use API and is easy to install.
It perform just as good as FMI Library, provides more features and is significantly easier to both build and use.


## Install instructions

Refer to [INSTALL.md](INSTALL.md)

## Build instructions

Refer to [BUILDING.md](BUILDING.md)

#### API

```cpp
#include <iostream> 
#include <fmi4cpp/fmi4cpp.hpp>

using namespace fmi4cpp;

const double stop = ...;
const double stepSize = ...;

int main() 
{
    fmi2::fmu fmu("path/to/fmu.fmu");

    auto cs_fmu = fmu.as_cs_fmu();
    auto me_fmu = fmu.as_me_fmu();
    
    auto cs_md = cs_fmu->get_model_description(); //smart pointer to a cs_model_description instance
    std::cout << "model_identifier=" << cs_md->model_identifier << std::endl;
    
    auto me_md = me_fmu->get_model_description(); //smart pointer to a me_model_description instance
    std::cout << "model_identifier=" << me_md->model_identifier << std::endl;
    
    auto var = cs_md->get_variable_by_name("my_var").as_real();
    std::cout << "Name=" << var.name() <<  ", start=" << var.start().value_or(0) << std::endl;
              
    auto slave = cs_fmu->new_instance();
    
    slave->setup_experiment();
    slave->enter_initialization_mode();
    slave->exit_initialization_mode();
    
    double t;
    double value;
    auto vr = var.valueReference();
    while ( (t = slave->get_simulation_time()) <= stop) {

        if (!slave->step(stepSize)) {
            std::cerr << "Error! step() returned with status: " << to_string(slave->last_status()) << std::endl;
            break;
        }
        
        if (!slave->read_real(vr, value)) {
            std::cerr << "Error! step() returned with status: " << to_string(slave->last_status()) << std::endl;
            break;
        }
        std::cout << "t=" << t << ", " << var.name() << "=" << value << std::endl;
     
    }
    
    slave->terminate();
    
    return 0;
}
```

*** 

Would you rather simulate or build FMUs in Java? Check out [FMI4j](https://github.com/NTNU-IHB/FMI4j)! <br>
Perhaps you want to build FMUs using plain Python? Check out [PythonFMU](https://github.com/NTNU-IHB/PythonFMU)! <br>
Need to distribute your FMUs? [FMU-proxy](https://github.com/NTNU-IHB/FMU-proxy) to the rescue! 


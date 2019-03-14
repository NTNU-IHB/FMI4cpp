# FMI4cpp (work in progress)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/NTNU-IHB/FMU-proxy/issues)
[![Join the chat at https://gitter.im/NTNU-IHB/FMI4cpp](https://badges.gitter.im/NTNU-IHB/FMI4cpp.svg)](https://gitter.im/NTNU-IHB/FMI4cpp?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)


[![CircleCI](https://circleci.com/gh/NTNU-IHB/FMI4cpp/tree/master.svg?style=svg)](https://circleci.com/gh/NTNU-IHB/FMI4cpp/tree/master)
[![Build Status](https://dev.azure.com/laht/laht/_apis/build/status/NTNU-IHB.FMI4cpp?branchName=master)](https://dev.azure.com/laht/laht/_build/latest?definitionId=3&branchName=master)

FMI4cpp is a cross-platform [FMI](https://fmi-standard.org/) 2.0 implementation written in modern C++.

Influenced by it's spiritual brother [FMI4j](https://github.com/NTNU-IHB/FMI4j), it aims to be
an easy to install, easy to use, object oriented and fast FMI implementation for C++.    

FMI4cpp supports both **Co-simulation** and **Model Exchange**. <br/>
For Model Exchange, solvers from [odeint](http://headmyshoulder.github.io/odeint-v2/doc/boost_numeric_odeint/getting_started/overview.html) can be used. 

### Why should I use this over other C/C++ FMI Libraries

Because it provides a clean, easy to use API and is easy to install (through vcpkg).
It perform just as good as FMI Library, provides more features and is significantly easier to both build and use.


## Install instructions

Refer to [INSTALL.md](INSTALL.md)

## Build instructions

Refer to [BUILDING.md](BUILDING.md)

#### API

```cpp
#include <iostream> 
#include <fmi4cpp/fmi4cpp.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

const double stop = ...;
const double stepSize = ...;

int main() {

    fmi2Fmu fmu("path/to/fmu.fmu");
    // or (with -DFMI4CPP_WITH_CURL=ON)
    // auto fmu = fmi2Fmu::fromUrl("http://somewebsite.org/somefmu.fmu")
    
    auto cs_fmu = fmu.asCoSimulationFmu();
    auto me_fmu = fmu.asModelExchangeFmu();
    
    auto cs_md = fmu.getModelDescription(); //smart pointer to a CoSimulationModelDescription instance
    cout << "modelIdentifier=" << cs_fmu->getModelDescription()->modelIdentifier << endl;
    
    auto me_md = fmu.getModelDescription(); //smart pointer to a ModelExchangeModelDescription instance
    cout << "modelIdentifier=" << me_fmu->getModelDescription()->modelIdentifier << endl;
    
    auto var = cs_md->getVariableByName("my_var").asReal();
    cout << "Name=" << var.name <<  ", start=" << var.start().value_or(0) << endl;
              
    auto slave = cs_fmu->newInstance();
    
    // or (with -DFMI4CPP_WITH_ODEINT=ON)
    // auto solver = make_solver<RK4ClassicSolver>(1E-3);
    // auto slave = me_fmu->newInstance(solver);
         
    slave->setupExperiment();
    slave->enterInitializationMode();
    slave->exitInitializationMode();
    
    double t;
    double value;
    while ( (t = slave->getSimulationTime()) <= stop) {

        if (!slave->doStep(stepSize)) {
            cerr << "Error! doStep() returned with status: " << to_string(slave->getLastStatus()) << endl;
            break;
        }
        
        if (!var.read(*slave, value)) {
            cerr << "Error! doStep() returned with status: " << to_string(slave->getLastStatus()) << endl;
            break;
        }
        cout << "t=" << t << ", " << var.name << "=" << value << endl;
     
    }
    
    slave->terminate();
    
    return 0;
    
}
```

----
### fmu_driver

FMI4cpp comes with a simple CLI for testing FMUs, called _fmu_driver_. 

```bash
Options:
  -h [ --help ]          Print this help message and quits.
  -f [ --fmu ] arg       Path to FMU.
  -o [ --output ] arg    Where to store the generated CSV results.
  --start arg            Start time.
  --stop arg             Stop time.
  --stepsize arg         StepSize.
  --me                   Treat FMU as an Model Exchange FMU.
  -v [ --variables ] arg Variables to print.
```


# FMI4cpp (work in progress)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/NTNU-IHB/FMU-proxy/issues)
[![Join the chat at https://gitter.im/NTNU-IHB/FMI4cpp](https://badges.gitter.im/NTNU-IHB/FMI4cpp.svg)](https://gitter.im/NTNU-IHB/FMI4cpp?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)


[![CircleCI](https://circleci.com/gh/NTNU-IHB/FMI4cpp/tree/master.svg?style=svg)](https://circleci.com/gh/NTNU-IHB/FMI4cpp/tree/master)
[![Build status](https://ci.appveyor.com/api/projects/status/7hrmwc7q6ake6jsd/branch/master?svg=true)](https://ci.appveyor.com/project/markaren/fmi4cpp/branch/master)

FMI4cpp is a cross-platform [FMI](https://fmi-standard.org/) 2.0 implementation written in modern C++.

Influenced by it's spiritual brother [FMI4j](https://github.com/NTNU-IHB/FMI4j), it aims to be
an easy to install, easy to use, object oriented and fast FMI implementation for C++.    

FMI4cpp supports both **Co-simulation** and **Model Exchange**. <br/>
For Model Exchange, solvers from [odeint](http://headmyshoulder.github.io/odeint-v2/doc/boost_numeric_odeint/getting_started/overview.html) can be used. 

### Why should I use this over other C/C++ FMI Libraries

Because it provides a clean, easy to use API and is easy to install (through vcpkg).
It perform just as good as FMI Library, provides more features and is significantly easier to both build and use.

### How do I use it in my own project?

#### vcpkg 

Install [vcpkg](https://github.com/Microsoft/vcpkg) and run:

```bash
./vcpkg install fmi4cpp
``` 

#### conan

Clone this repository and run:

```bash
conan create . FMI4cpp/local
```

### Development dependencies

You are free to use either `vcpkg`, `conan` or `apt-get` for getting the required dependencies

##### vcpkg

Install [vcpkg](https://github.com/Microsoft/vcpkg) and run:

```bash
./vcpkg install boost-property-tree libzip[core] spdlog
``` 

On Windows you might want to specify the target architecture (defaults to x86) by appending:

```bash
--triplet [x86-windows|x64-windows]
```

Then tell CMake about your vcpkg installation by passing <br> ```-DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake``` to it.

When using vcpkg, set `-DFMI4CPP_USING_VCPKG=ON`

##### conan

Install [conan](https://conan.io/) and run `conan install`:

```bash
conan install . -s build_type=Debug --install-folder=cmake-build-debug
conan install . -s build_type=Release --install-folder=cmake-build-release
```

On Linux you should add `-s compiler.libcxx=libstdc++11` to the command.

When using conan, set `FMI4CPP_USING_CONAN=ON`.

##### apt-get

```bash
Linux:~/$ sudo apt-get install libzip-dev libboost-dev libspdlog-dev
``` 


## Optional features

#### Load FMUs from URL

Set `-DFMI4CPP_WITH_CURL=ON`
Requires libcurl

##### vcpkg
`./vcpkg install curl`

##### conan
pass `-o curl=True` to the install command

##### apt-get
```bash
sudo apt-get install libcurl4-openssl-dev
``` 

#### Wrap Model Exchange models as Co-simulation models

Set `-DFMI4CPP_WITH_ODEINT=ON`
Requires boost-odeint

##### vcpkg
`./vcpkg install boost-ublas boost-odeint` 

##### conan & apt-get
_Dependencies should already be fulfilled.._

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

It's not built by default. To build it, pass ```FMI4CPP_BUILD_TOOL=ON``` to CMake. 
An additional dependency is required if you do:

##### vcpkg
```bash
./vcpkg install boost-program-options
```
##### conan
_Dependency should already be fulfilled.._

##### apt-get
```bash
Linux:~/$ sudo apt-get install libboost-program-options-dev
```

----

#### Running examples/tests

The examples are built by default. To change this pass ```-DFMI4CPP_BUILD_EXAMPLES=OFF``` to CMake.
Test are on the other hand _not_ built by default. To change this pass ```-DFMI4CPP_BUILD_TESTS=ON``` to CMake.

To run the tests you will need an additional dependency:

##### vcpkg
```
./vcpkg install boost-test
``` 
##### conan
_Dependency should already be fulfilled.._

##### apt-get
```bash
Linux:~/$ sudo apt-get install libboost-test-dev
```

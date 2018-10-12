# FMI4cpp (work in progress)

FMI4cpp is a cross-platform FMI 2.0 implementation written in modern C++.

Influenced by it's spiritual brother [FMI4j](https://github.com/SFI-Mechatronics/FMI4j), it aims to be
an easy to install, easy to use, object oriented and fast FMI implementation for C++.    

### Why should I use this over other C/C++ FMI Libraries

Because this library provides a clean API, is easy to install (through vcpkg) and is easy to use.


### How do I use it in my own project?

When the library is stable it will be added to the vcpkg package manager.


### Development dependencies

Install [vcpkg](https://github.com/Microsoft/vcpkg) and run:

```
./vcpkg install boost-property-tree libzip
``` 

On linux you _might_ also need to install some additional libraries:

```
./vcpkg install bzip2 openssl
``` 

These can probably also be installed through the native package handler. 

#### API

```cpp

#include <iostream>
#include <fmi4cpp/fmi4cpp.hpp>

using namespace fmi4cpp::fmi2;

const double stop = 10.0;
const double stepSize = 1.0/100;

int main() {

    import::Fmu fmu("path/to/fmu.fmu");
    
    auto& md = fmu.getModelDescription();
    xml::ScalarVariable var = md->getVariableByName("my_var");
    
    auto md_cs = md.asCoSimulationModelDescription();
    std::cout << "modelIdentifier=" << md_cs.getModelIdentifier() << std::endl;
    
    auto slave = fmu.asCoSimulationFmu()->newInstance();
    slave->init();
   
    double t;
    fmi2Real value;
    fmi2Status status;
    fmi2ValueReference vr = var.valueReference();
    while ( (t = slave->getSimulationTime()) <= stop) {
    
        status = slave->doStep(stepSize);
        if (status != fmi2OK) {
            //error handling
            break;
        }
        
        status = slave->readReal(vr, value);
        if (status != fmi2OK) {
            //error handling
            break;
        }
        std::cout << "t=" << t << ", " << var.getName() << "=" << value << std::endl;
     
    }
    
    slave->terminate();
    
}
```

***

#### Running examples/tests

In order to run the example/test code, a system variable named __TEST_FMUs__ must be present on your system. 
This variable should point to the location of the content found [here](https://github.com/markaren/TEST_FMUs).

The examples are built by default. To change this pass ```-DBUILD_EXAMPLES=OFF``` to CMmake.
Test are on the other hand _not_ built by default. To change this pass ```-DBUILD_TESTS=ON``` to CMake.

To run the tests you will need an additional dependency:

```
./vcpkg install boost-test
``` 
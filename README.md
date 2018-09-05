# FMI4cpp (work in progress)

FMI4cpp is a cross-platform FMI 2.0 implementation written in modern C++.

Looking for an easy to install, easy to use, easy to reason with, object oriented and fast FMI implementation for C++? 
You are in luck. FMI4cpp is all of those.

### Why should I use this over other C/C++ FMI Libraries

Because this library has a much cleaner API and is significantly easier to use.


### How do I use it in my own project?

When the library is stable I will add it to vcpkg.


### Development dependencies

Install [vcpkg](https://github.com/Microsoft/vcpkg) and run

```
./vcpkg install boost-property-tree boost-filesystem libzip
``` 

#### API

```cpp

#include <iostream>
#include <fmicpp/fmicpp.hpp>

using namespace fmicpp::fmi2;

int main() {

    import::Fmu fmu("path/to/fmu.fmu");
    auto slave = fmu.asCoSimulationFmu().newInstance();
    
    xml::ScalarVariable& var = md->getVariableByName("my_var");
    
    slave->init();
    
    double stop = 10.0;
    double stepSize = 1.0/100;
    
    double t;
    fmi2Status status;
    fmi2Real value;
    while ( (t = slave->getSimulationTime()) <= stop) {
    
        status = slave->doStep(stepSize);
        if (status != fmi2OK) {
            //error handling
            break;
        }
        
        status = slave->readReal(var.valueReference, value);
        if (status != fmi2OK) {
            //error handling
            break;
        }
        std::cout << "t=" << t << ", " << var.name << "=" << value << std::endl;
     
    }
    
    slave->terminate();
    
}

```
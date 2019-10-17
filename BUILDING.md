### BUILDING

For building FMI4cpp, you are can use either `conan` or `apt-get` for getting the required dependencies.

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
Linux:~/$ sudo apt-get install libzip-dev libboost-dev libbbost-filesystem-dev
``` 


## Optional features

#### Wrap Model Exchange models as Co-simulation models

Set `-DFMI4CPP_WITH_ODEINT=ON`


### fmu_driver

The fmu_driver tool is not built by default. To build it, pass ```FMI4CPP_BUILD_TOOL=ON``` to CMake. 
An additional dependency is required if you do:

##### conan
_Dependency should already be fulfilled.._

##### apt-get
```bash
Linux:~/$ sudo apt-get install libboost-program-options-dev
```

---


### Running examples/tests

The examples are built by default. To change this pass ```-DFMI4CPP_BUILD_EXAMPLES=OFF``` to CMake.
Test are on the other hand _not_ built by default. To change this pass ```-DFMI4CPP_BUILD_TESTS=ON``` to CMake.

To run the tests you will need an additional dependency:

##### conan
_Dependency should already be fulfilled.._

##### apt-get
```bash
Linux:~/$ sudo apt-get install libboost-test-dev
```

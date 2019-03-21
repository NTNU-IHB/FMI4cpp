### BUILDING

For building FMI4cpp, you are free to use either `vcpkg`, `conan` or `apt-get` for getting the required dependencies.

##### vcpkg

Install [vcpkg](https://github.com/Microsoft/vcpkg) and run:

```bash
./vcpkg install boost-property-tree libzip[core]
``` 

On Windows you might want to specify the target architecture (defaults to x86) by appending:

```bash
--triplet [x86-windows|x64-windows]
```

Then tell CMake about your vcpkg installation by passing <br> ```-DCMAKE_TOOLCHAIN_FILE=<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake``` to it.

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
Linux:~/$ sudo apt-get install libzip-dev libboost-dev
``` 


## Optional features

#### Load FMUs from URL

Set `-DFMI4CPP_WITH_CURL=ON`

This feature requires libcurl:

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

This feature requires boost-odeint:

##### vcpkg
`./vcpkg install boost-ublas boost-odeint` 

##### conan & apt-get
_Dependencies should already be fulfilled.._



### fmu_driver

The fmu_driver tool is not built by default. To build it, pass ```FMI4CPP_BUILD_TOOL=ON``` to CMake. 
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

---


### Running examples/tests

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

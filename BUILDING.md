### BUILDING

For building FMI4cpp, you can use either `conan` or `apt-get` for getting the required dependencies.

##### conan

Install [conan](https://conan.io/) and run cmake with the option `FMI4CPP_USING_CONAN=ON`.

##### apt-get

```bash
Linux:~/$ sudo apt-get install libzip-dev libboost-dev
``` 

### Running examples/tests

The examples are built by default. To change this pass ```-DFMI4CPP_BUILD_EXAMPLES=OFF``` to CMake.
Test are on the other hand _not_ built by default. To change this pass ```-DFMI4CPP_BUILD_TESTS=ON``` to CMake.

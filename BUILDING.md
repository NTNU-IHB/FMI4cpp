### BUILDING

For building FMI4cpp, you can use either `conan` or `apt-get` for getting the required dependencies.

##### conan

Install [conan](https://conan.io/) and run cmake with the option `-DFMI4CPP_USING_CONAN=ON`.

##### apt-get

```bash
Linux:~/$ sudo apt-get install libzip-dev
``` 

### Running examples/tests

To build the examples pass `-DFMI4CPP_BUILD_EXAMPLES=ON` to CMake.
To build the tests pass `-DFMI4CPP_BUILD_TESTS=ON` to CMake.

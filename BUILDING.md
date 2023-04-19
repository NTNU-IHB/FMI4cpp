### BUILDING

For building FMI4cpp, you can use either `vcpkg` or `apt-get` for getting the required dependencies.

##### vcpkg

Call CMake with `-DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake`

##### apt-get

```bash
Linux:~/$ sudo apt-get install libzip-dev libpugixml-dev
``` 

### Running examples/tests

To build the examples pass `-DFMI4CPP_BUILD_EXAMPLES=ON` to CMake.
To build the tests pass `-DFMI4CPP_BUILD_TESTS=ON` to CMake.

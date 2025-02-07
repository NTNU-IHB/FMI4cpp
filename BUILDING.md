### BUILDING

FMI4cpp does not need any additional dependencies. For unzipping in depends on system tools.
This should be available on Windows, on linux you might need to install `unzip``.

```bash
Linux:~/$ sudo apt-get install unzip
``` 

For XML handling, it ships with a version of pugiXML. If you want to use an external version then pass
`-DFMI4CPP_USE_EXTERNAL_PUGIXML=ON` to CMake.

#### External pgiXML (Optional) 
To get an external pugiXML version you may use `vcpkg` or `apt-get`.

##### apt-get

```bash
Linux:~/$ sudo apt-get install libpugixml-dev
``` 

### Running examples/tests

To build the examples pass `-DFMI4CPP_BUILD_EXAMPLES=ON` to CMake.
To build the tests pass `-DFMI4CPP_BUILD_TESTS=ON` to CMake.

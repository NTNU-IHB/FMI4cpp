
### INSTALL

FMI4cpp can be installed using the `vcpkg` and `conan` package managers. See below. 


#### vcpkg 

Install [vcpkg](https://github.com/Microsoft/vcpkg) and run:

```bash
./vcpkg install fmi4cpp
``` 

#### conan

Clone this repository and run:

```bash
conan create . FMI4cpp/testing
```


Additionally, you can use it as a submodule in your own CMake project.
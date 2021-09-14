
### INSTALL

`fmi4cpp` is available through the conan remote ais:

`conan remote add ais https://ais.jfrog.io/artifactory/api/conan/ais-conan-local`

Then add a dependency to: </br>
`fmi4cpp/<version>@ais/stable` (stable channel -> releases) </br>
`fmi4cpp/<version>@ais/testing` (development builds -> master) </br>
`fmi4cpp/<version>@ais/testing-<branch>` (development builds -> branches)


Additionally, you can of course build, include and link to it as any other 
CMake project or include it as a subproject in your own CMake project.


include_directories(${FMI4cpp_SOURCE_DIR}/include)
include_directories(include)

add_library(fmu_driver_lib
        src/FmuDriver.cpp)
target_link_libraries(fmu_driver_lib FMI4cpp::fmi4cpp)

add_executable(fmu_driver
        cmd_parser.cpp)
target_link_libraries(fmu_driver fmu_driver_lib Boost::program_options)

install(TARGETS fmu_driver DESTINATION bin)

add_subdirectory(crosscheck)
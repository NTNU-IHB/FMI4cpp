
include_directories(../include)

add_executable(fmu_driver
        DriverOptions.hpp
        FmuDriver.cpp
        FmuDriver.hpp
        cmd_parser.cpp)
target_link_libraries(fmu_driver FMI4cpp::fmi4cpp Boost::program_options)

install(TARGETS fmu_driver DESTINATION bin)

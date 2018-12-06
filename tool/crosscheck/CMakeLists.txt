
include_directories(${FMI4cpp_SOURCE_DIR}/include)
include_directories(${FMI4cpp_SOURCE_DIR}/src/)
include_directories(../include)


add_executable(fmi4cpp_crosscheck
        crosscheck.cpp)
target_link_libraries(fmi4cpp_crosscheck fmu_driver_lib FMI4cpp::fmi4cpp)

install(TARGETS fmi4cpp_crosscheck DESTINATION bin)

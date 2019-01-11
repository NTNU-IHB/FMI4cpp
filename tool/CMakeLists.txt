
include_directories(${FMI4cpp_SOURCE_DIR}/include)

add_executable(fmu_driver
        fmu_driver.cpp)
target_link_libraries(fmu_driver FMI4cpp::fmi4cpp Boost::program_options)

add_executable(fmi4cpp_crosscheck
        crosscheck.cpp)
target_link_libraries(fmi4cpp_crosscheck FMI4cpp::fmi4cpp)
target_include_directories(fmi4cpp_crosscheck PRIVATE ${FMI4cpp_SOURCE_DIR}/src)

install(TARGETS fmu_driver DESTINATION bin)
install(TARGETS fmi4cpp_crosscheck DESTINATION bin)


include_directories(${FMI4cpp_SOURCE_DIR}/include)

add_executable(fmu_driver
        fmu_driver.cpp)
target_link_libraries(fmu_driver FMI4cpp::fmi4cpp Boost::program_options)

install(TARGETS fmu_driver DESTINATION bin)

add_subdirectory(crosscheck)
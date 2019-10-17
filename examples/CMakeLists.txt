
link_libraries(fmi4cpp::fmi4cpp)
include_directories(${fmi4cpp_SOURCE_DIR}/src/fmi4cpp/tools)

add_executable(fmu_test fmu_test.cpp)
add_executable(torsionbar torsionbar.cpp)
add_executable(multiple_fmus multiple_fmus.cpp)
add_executable(controlled_temperature controlled_temperature.cpp)


include_directories(../include)
link_libraries(fmi4cpp)

add_executable(fmu_test fmu_test.cpp)
add_executable(torsionbar torsionbar.cpp)
add_executable(controlled_temperature controlled_temperature.cpp)

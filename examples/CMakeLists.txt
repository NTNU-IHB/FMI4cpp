
include_directories(${FMI4CPP_SOURCE_DIR}/include)

add_executable(fmu_test fmu_test.cpp)
target_link_libraries(fmu_test fmi4cpp)

add_executable(controlled_temperature controlled_temperature.cpp)
target_link_libraries(controlled_temperature fmi4cpp)

add_executable(torsionbar torsionbar.cpp)
target_link_libraries(torsionbar fmi4cpp)


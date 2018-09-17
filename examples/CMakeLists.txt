
include_directories(${FMICPP_SOURCE_DIR}/include)

add_executable(fmu_test fmu_test.cpp)
target_link_libraries(fmu_test fmicpp)

add_executable(controlled_temperature controlled_temperature.cpp)
target_link_libraries(controlled_temperature fmicpp)

add_executable(torsionbar torsionbar.cpp)
target_link_libraries(torsionbar fmicpp)


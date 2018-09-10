
include_directories(${FMICPP_SOURCE_DIR}/include)

add_executable(fmu_test fmu_test.cpp)
target_link_libraries(fmu_test fmicpp)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark fmicpp)


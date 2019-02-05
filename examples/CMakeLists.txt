
include_directories(../src)
link_libraries(FMI4cpp::fmi4cpp)

add_executable(fmu_test fmu_test.cpp)
add_executable(torsionbar torsionbar.cpp)
add_executable(multiple_fmus multiple_fmus.cpp)
add_executable(controlled_temperature controlled_temperature.cpp)

if (FMI4CPP_WITH_ODEINT)
    add_executable(mewrapper_test mewrapper_test.cpp)
endif ()

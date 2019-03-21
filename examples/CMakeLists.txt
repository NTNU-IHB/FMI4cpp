
link_libraries(fmi4cpp::fmi4cpp)
include_directories(${fmi4cpp_SOURCE_DIR}/src/fmi4cpp/tools)

add_executable(fmu_test fmu_test.cpp)
add_executable(torsionbar torsionbar.cpp)
add_executable(multiple_fmus multiple_fmus.cpp)
add_executable(controlled_temperature controlled_temperature.cpp)

if (FMI4CPP_WITH_CURL)
    add_executable(from_url from_url.cpp)
endif ()

if (FMI4CPP_WITH_ODEINT)
    add_executable(mewrapper_test mewrapper_test.cpp)
endif ()

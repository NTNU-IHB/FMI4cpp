
include_directories(${FMI4cpp_SOURCE_DIR}/include)
link_libraries(fmi4cpp)

if (UNIX AND FMI4CPP_USING_VCPKG AND FMI4CPP_WITH_CURL)
    link_libraries(${CURL_LIBRARIES} OpenSSL::Crypto OpenSSL::SSL)
endif ()

add_executable(fmu_driver fmu_driver.cpp)
target_link_libraries(fmu_driver PRIVATE Boost::program_options)

add_executable(fmi4cpp_crosscheck crosscheck.cpp)
target_include_directories(fmi4cpp_crosscheck PRIVATE ${FMI4cpp_SOURCE_DIR}/src)

install(TARGETS fmu_driver DESTINATION bin)
install(TARGETS fmi4cpp_crosscheck DESTINATION bin)


include_directories(../src ${Boost_INCLUDE_DIRS})
link_libraries(FMI4cpp::fmi4cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

if (UNIX AND FMI4CPP_USING_VCPKG AND FMI4CPP_WITH_CURL)
    link_libraries(${CURL_LIBRARIES} OpenSSL::Crypto OpenSSL::SSL)
endif ()

enable_testing()

add_executable(test_controlled_temperature test_controlled_temperature.cpp)
add_test(NAME test_controlled_temperature COMMAND test_controlled_temperature)

add_executable(test_model_description1 test_modeldescription1.cpp)
add_test(NAME test_model_description1 COMMAND test_model_description1)

add_executable(test_model_description2 test_modeldescription2.cpp)
add_test(NAME test_model_description2 COMMAND test_model_description2)

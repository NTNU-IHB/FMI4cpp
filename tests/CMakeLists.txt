
link_directories(${Boost_INCLUDE_DIRS})
link_libraries(fmi4cpp::fmi4cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

enable_testing()

add_executable(test_controlled_temperature test_controlled_temperature.cpp)
target_compile_definitions(test_controlled_temperature PRIVATE "BOOST_ALL_DYN_LINK=1" "BOOST_ALL_NO_LIB=1" "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1")
add_test(NAME test_controlled_temperature COMMAND test_controlled_temperature)

add_executable(test_model_description1 test_modeldescription1.cpp)
target_compile_definitions(test_model_description1 PRIVATE "BOOST_ALL_DYN_LINK=1" "BOOST_ALL_NO_LIB=1" "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1")
add_test(NAME test_model_description1 COMMAND test_model_description1)

add_executable(test_model_description2 test_modeldescription2.cpp)
target_compile_definitions(test_model_description2 PRIVATE "BOOST_ALL_DYN_LINK=1" "BOOST_ALL_NO_LIB=1" "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1")
add_test(NAME test_model_description2 COMMAND test_model_description2)

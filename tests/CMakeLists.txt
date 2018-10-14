
include_directories(../src ${Boost_INCLUDE_DIRS})
link_libraries(FMI4cpp::fmi4cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable(test_model_description1 test_modeldescription1.cpp)
add_executable(test_model_description2 test_modeldescription2.cpp)
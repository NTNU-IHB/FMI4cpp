
include_directories(${FMICPP_SOURCE_DIR}/include)
link_libraries(fmicpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable(test_model_description1 test_modeldescription1.cpp)

include_directories(${FMICPP_SOURCE_DIR}/include)

add_executable(test_parse_xml test_parse_xml.cpp)
target_link_libraries(test_parse_xml fmicpp)

add_executable(fmu_test fmu_test.cpp)
target_link_libraries(fmu_test fmicpp)

add_executable(zip_test zip_test.cpp)
target_link_libraries(zip_test Boost::system Boost::filesystem ${LIBZIP_LIBRARIES})
target_include_directories(zip_test PRIVATE ${LIBZIP_INCLUDE_DIR})

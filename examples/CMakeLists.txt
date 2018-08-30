
include_directories(${FMICPP_SOURCE_DIR}/include)

add_executable(test_parse_xml test_parse_xml.cpp)
target_link_libraries(test_parse_xml fmicpp)

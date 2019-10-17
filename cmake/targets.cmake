
add_subdirectory(src)

if (FMI4CPP_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if (FMI4CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (FMI4CPP_BUILD_TESTS OR FMI4CPP_BUILD_EXAMPLES)
    file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

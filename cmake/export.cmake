
set(FMI4CPP_HEADER_INSTALL_DIR "include")
set(FMI4CPP_CMAKE_INSTALL_DIR  "share/${PROJECT_NAME}")
set(FMI4CPP_DOC_INSTALL_DIR    "share/doc/${PROJECT_NAME}")

set(FMI4CPP_INSTALL_DESTINATIONS
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib"
        RUNTIME DESTINATION "bin"
        INCLUDES DESTINATION "${FMI4CPP_HEADER_INSTALL_DIR}")
set(FMI4CPP_EXPORT_TARGET "${PROJECT_NAME}-targets")

# Add uninstall logic (see https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake)
if (NOT TARGET uninstall)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
            IMMEDIATE @ONLY)

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

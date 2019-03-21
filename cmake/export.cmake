
install(
        EXPORT "${FMI4CPP_EXPORT_TARGET}"
        DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}"
        NAMESPACE "${PROJECT_NAME}::"
)

include(CMakePackageConfigHelpers)

# Generate and install package-config file.
set(configFile "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(targetsFile "${FMI4CPP_CMAKE_INSTALL_DIR}/${FMI4CPP_EXPORT_TARGET}.cmake")
configure_package_config_file(
        "${CMAKE_SOURCE_DIR}/cmake/project-config.cmake.in"
        "${configFile}"
        INSTALL_DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}"
        PATH_VARS targetsFile
)
install(FILES "${configFile}" DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}")

# Generate and install package-version file
set(versionFile "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
write_basic_package_version_file(
        "${versionFile}"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY "SameMajorVersion")
install(FILES "${versionFile}" DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}")
cmake_minimum_required(VERSION 3.15)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" projectVersion)
project(fmi4cpp VERSION ${projectVersion})
message("Current fmi4cpp version: ${projectVersion}\n")


# ==============================================================================
# Build settings
# ==============================================================================

option(FMI4CPP_BUILD_TESTS "Build tests" OFF)
option(FMI4CPP_BUILD_EXAMPLES "Build examples" OFF)
option(FMI4CPP_USING_CONAN "Build using conan" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" ON)

set(FMI4CPP_LOG_LEVEL "DEFAULT" CACHE STRING "FMI4cpp logging level")
set(FMI4CPP_LOG_LEVEL_VALUES "DEFAULT;OFF;TRACE;DEBUG;INFO;WARN;ERROR;FATAL" CACHE STRING "List of possible log levels")
set_property(CACHE FMI4CPP_LOG_LEVEL PROPERTY STRINGS ${FMI4CPP_LOG_LEVEL_VALUES})


# ==============================================================================
# Global internal configuration
# ==============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif ()

if (FMI4CPP_USING_CONAN)
    if (CONAN_EXPORTED) # in conan local cache
        # standard conan installation, deps will be defined in conanfile.py
        # and not necessary to call conan again, conan is already running
        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        conan_basic_setup()
    else ()

        include(cmake/conan.cmake)
        conan_cmake_autodetect(settings)

        set(shared False)
        if (BUILD_SHARED_LIBS)
            set(shared True)
        endif ()

        conan_cmake_run(
                CONANFILE conanfile.py
                OPTIONS shared=${shared}
                BUILD missing
                SETTINGS ${settings}
                BASIC_SETUP)

    endif ()
endif ()

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions("-D_CRT_SECURE_NO_WARNINGS")
    add_compile_options(/wd4996) # suppress warning appearing in boost headers under MSVC.
    #https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4996?view=vs-2017
endif ()

include(GNUInstallDirs)

set(FMI4CPP_HEADER_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(FMI4CPP_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")
set(FMI4CPP_DOC_INSTALL_DIR "${CMAKE_INSTALL_DOCDIR}")

set(FMI4CPP_INSTALL_DESTINATIONS
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib"
        RUNTIME DESTINATION "bin"
        INCLUDES DESTINATION "${FMI4CPP_HEADER_INSTALL_DIR}")
set(FMI4CPP_EXPORT_TARGET "${PROJECT_NAME}-targets")


# ==============================================================================
# Dependencies
# ==============================================================================

find_package(LIBZIP REQUIRED)
find_package(PugiXML REQUIRED)

if (FMI4CPP_BUILD_TESTS)

    Include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v2.13.8
    )

    FetchContent_MakeAvailable(Catch2)
endif()


# ==============================================================================
# Targets
# ==============================================================================

add_subdirectory(src)

if (FMI4CPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

if (FMI4CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (FMI4CPP_BUILD_TESTS OR FMI4CPP_BUILD_EXAMPLES)
    file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif ()


# ==============================================================================
# Exports and remaining installation
# ==============================================================================

install(
        EXPORT "${FMI4CPP_EXPORT_TARGET}"
        DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}"
        NAMESPACE "${PROJECT_NAME}::"
)

include(CMakePackageConfigHelpers)

# Generate and install package-config file.
set(configFile "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(targetsFile "${FMI4CPP_CMAKE_INSTALL_DIR}/${FMI4CPP_EXPORT_TARGET}.cmake")
configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
        "${configFile}"
        INSTALL_DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}"
        PATH_VARS targetsFile
)
install(FILES "${configFile}" DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}")

# Generate and install package-version file
set(versionFile "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
write_basic_package_version_file(
        "${versionFile}"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY "SameMajorVersion")
install(FILES "${versionFile}" DESTINATION "${FMI4CPP_CMAKE_INSTALL_DIR}")


# Install custom find modules
install(FILES
        ${PROJECT_SOURCE_DIR}/cmake/FindLIBZIP.cmake
        ${PROJECT_SOURCE_DIR}/cmake/FindPugiXML.cmake
        DESTINATION
        ${FMI4CPP_CMAKE_INSTALL_DIR}
        )

cmake_minimum_required(VERSION 3.10)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" projectVersion)
project(fmi4cpp VERSION ${projectVersion})
message("Current fmi4cpp version: ${projectVersion}\n")


option(FMI4CPP_BUILD_TESTS "Build tests" OFF)
option(FMI4CPP_BUILD_EXAMPLES "Build examples" OFF)
option(FMI4CPP_USING_CONAN "Build using conan" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries instead of static libraries" ON)

set(FMI4CPP_LOG_LEVEL "DEFAULT" CACHE STRING "FMI4cpp logging level")
set(FMI4CPP_LOG_LEVEL_VALUES "DEFAULT;OFF;TRACE;DEBUG;INFO;WARN;ERROR;FATAL" CACHE STRING "List of possible log levels")
set_property(CACHE FMI4CPP_LOG_LEVEL PROPERTY STRINGS ${FMI4CPP_LOG_LEVEL_VALUES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(FMI4CPP_USING_CONAN)
    if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        conan_basic_setup()
    else()
        message(FATAL_ERROR "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first!")
    endif()
endif()

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions("-DNOMINMAX" "-D_CRT_SECURE_NO_WARNINGS")
    add_compile_options(/wd4996) # suppress warning appearing in boost headers under MSVC.
    #https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4996?view=vs-2017
endif ()

include(export)
include(dependencies)
include(targets)
include(install)

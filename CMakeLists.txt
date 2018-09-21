
cmake_minimum_required(VERSION 3.10)

SET(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE STRING "VCPKG_ROOT")
if (DEFINED VCPKG_ROOT AND NOT DEFINED CMAKE_TOOLCHAIN_FILE AND EXISTS "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif ()

project(FMI4cpp
        VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" ON)

string(COMPARE EQUAL ${CMAKE_BUILD_TYPE} "Debug" DEBUG_MODE)
option(DEBUG_LOGGING_ENABLED "Enable debug logging" ${DEBUG_MODE})
message("Debug logging=${DEBUG_LOGGING_ENABLED}")

if (DEBUG_LOGGING_ENABLED)
    add_definitions(-DDEBUG_LOGGING_ENABLED=1)
else ()
    add_definitions(-DDEBUG_LOGGING_ENABLED=0)
endif ()

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

if (BUILD_TESTS)
    SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} unit_test_framework)
endif ()
find_package(Boost 1.65 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)


if (UNIX)
    find_package(ZLIB REQUIRED)
    find_package(BZip2 REQUIRED)
    find_package(OpenSSL COMPONENTS Crypto REQUIRED)
endif ()

find_package(LIBZIP REQUIRED)

add_subdirectory(src)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

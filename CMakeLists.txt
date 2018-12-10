
cmake_minimum_required(VERSION 3.10)
project(FMI4cpp
        VERSION 0.4.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(FMI4CPP_BUILD_TOOL "Build tool" ON)
option(FMI4CPP_BUILD_TESTS "Build tests" OFF)
option(FMI4CPP_BUILD_EXAMPLES "Build examples" ON)
option(FMI4CPP_WITH_ODEINT "Build with odeint solvers" ON)

if (MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/wd4996) # suppress warning appearing in boost headers under MSVC.
    #https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4996?view=vs-2017
endif ()

if (FMI4CPP_BUILD_TOOL)
    SET(BOOST_COMPONENTS ${BOOST_COMPONENTS} program_options)
endif ()
if (FMI4CPP_BUILD_TESTS)
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


if (FMI4CPP_BUILD_TOOL)
    add_subdirectory(tool)
endif ()

if (FMI4CPP_BUILD_TESTS)
    add_subdirectory(tests)
endif ()

if (FMI4CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()


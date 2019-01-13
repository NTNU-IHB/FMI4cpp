
cmake_minimum_required(VERSION 3.10)
project(FMI4cpp
        VERSION 0.5.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(FMI4CPP_BUILD_TOOL "Build tool" ON)
option(FMI4CPP_BUILD_TESTS "Build tests" OFF)
option(FMI4CPP_BUILD_EXAMPLES "Build examples" ON)
option(FMI4CPP_WITH_ODEINT "Build with odeint solvers" ON)
option(FMI4CPP_USING_VCPKG "Build using vcpkg" ON)

set(FMI4CPP_LOG_LEVEL "DEFAULT" CACHE STRING "FMI4cpp logging level")
set(FMI4CPP_LOG_LEVEL_VALUES "DEFAULT;OFF;INFO;DEBUG;TRACE" CACHE STRING "List of possible log levels")
set_property(CACHE FMI4CPP_LOG_LEVEL PROPERTY STRINGS ${FMI4CPP_LOG_LEVEL_VALUES})

if (FMI4CPP_LOG_LEVEL STREQUAL "OFF")
    add_definitions(-DFMI4CPP_LOG_LEVEL_OFF)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "INFO")
    add_definitions(-DFMI4CPP_LOG_LEVEL_INFO)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "DEBUG")
    add_definitions(-DFMI4CPP_LOG_LEVEL_DEBUG)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "DEBUG")
    add_definitions(-DFMI4CPP_LOG_LEVEL_TRACE)
else ()
    add_definitions(-DFMI4CPP_LOG_LEVEL_DEFAULT)
endif ()

add_definitions(-DFMT_HEADER_ONLY)

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

if (FMI4CPP_USING_VCPKG AND UNIX)
    find_package(ZLIB REQUIRED)
    find_package(BZip2 REQUIRED)
endif ()

find_package(LIBZIP REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src)

if (FMI4CPP_BUILD_TOOL)
    add_subdirectory(tool)
endif ()

if (FMI4CPP_BUILD_TESTS OR FMI4CPP_BUILD_EXAMPLES)
    file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

if (FMI4CPP_BUILD_TESTS)
    if (NOT FMI4CPP_USING_VCPKG)
        add_definitions(-DBOOST_TEST_DYN_LINK)
    endif()
    add_subdirectory(tests)
endif ()

if (FMI4CPP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()


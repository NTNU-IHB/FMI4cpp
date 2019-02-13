
option(FMI4CPP_BUILD_TOOL "Build tool" OFF)
option(FMI4CPP_BUILD_TESTS "Build tests" OFF)
option(FMI4CPP_BUILD_EXAMPLES "Build examples" ON)
option(FMI4CPP_WITH_ODEINT "Build with odeint solvers. Allows ME models to be wrapped as CS ones" OFF)
option(FMI4CPP_WITH_CURL "Build with curl. Allows URL paths to FMUs" OFF)
option(FMI4CPP_USING_VCPKG "Build using vcpkg" ON)
option(FMI4CPP_USING_CONAN "Build using conan" OFF)

if (FMI4CPP_USING_VCPKG AND FMI4CPP_USING_CONAN)
    message(FATAL_ERROR "Cannot use both vcpkg and conan!")
endif()

if(NOT FMI4CPP_USING_VCPKG)
    set(CMAKE_TOOLCHAIN_FILE "")
endif()

cmake_minimum_required(VERSION 3.10)
project(FMI4cpp
        VERSION 0.5.4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


set(FMI4CPP_LOG_LEVEL "DEFAULT" CACHE STRING "FMI4cpp logging level")
set(FMI4CPP_LOG_LEVEL_VALUES "DEFAULT;OFF;INFO;DEBUG;TRACE" CACHE STRING "List of possible log levels")
set_property(CACHE FMI4CPP_LOG_LEVEL PROPERTY STRINGS ${FMI4CPP_LOG_LEVEL_VALUES})

if (FMI4CPP_USING_CONAN)
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

if (FMI4CPP_BUILD_TOOL)
    list(APPEND BOOST_COMPONENTS program_options)
endif ()
if (FMI4CPP_BUILD_TESTS)
    list(APPEND BOOST_COMPONENTS unit_test_framework)
endif ()
find_package(Boost 1.65 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

if (FMI4CPP_USING_VCPKG)
    if (UNIX)
        find_package(ZLIB REQUIRED)
        find_package(BZip2 REQUIRED)
        if (FMI4CPP_WITH_CURL)
            find_package(OpenSSL COMPONENTS Crypto SSL REQUIRED)
        endif()
    endif()
endif ()

find_package(LIBZIP REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if (FMI4CPP_WITH_CURL)
    find_package(CURL REQUIRED)
endif()

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

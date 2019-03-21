
if (FMI4CPP_BUILD_TOOL)
    list(APPEND BOOST_COMPONENTS program_options)
endif ()

if (FMI4CPP_BUILD_TESTS)
    list(APPEND BOOST_COMPONENTS unit_test_framework)
endif ()

find_package(Boost 1.65 COMPONENTS ${BOOST_COMPONENTS} REQUIRED)

find_package(LIBZIP REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if (FMI4CPP_WITH_CURL)
    find_package(CURL_ REQUIRED)
endif()

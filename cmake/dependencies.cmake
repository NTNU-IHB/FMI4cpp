
if (FMI4CPP_BUILD_TOOL)
    list(APPEND Boost_COMPONENTS program_options)
endif()

if (FMI4CPP_BUILD_TESTS)
    list(APPEND Boost_COMPONENTS unit_test_framework)
endif()

find_package(Boost 1.65 COMPONENTS ${Boost_COMPONENTS} REQUIRED)
find_package(LIBZIP REQUIRED)

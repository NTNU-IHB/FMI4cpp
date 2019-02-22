
SET(SOURCES

        fmi4cpp/common/import/FmuResource.cpp
        fmi4cpp/common/driver/fmu_driver.cpp

        fmi4cpp/fmi2/import/fmi2Fmu.cpp
        fmi4cpp/fmi2/import/fmi2CoSimulationFmu.cpp
        fmi4cpp/fmi2/import/fmi2ModelExchangeFmu.cpp
        fmi4cpp/fmi2/import/fmi2Library.cpp
        fmi4cpp/fmi2/import/fmi2CoSimulationLibrary.cpp
        fmi4cpp/fmi2/import/fmi2ModelExchangeLibrary.cpp
        fmi4cpp/fmi2/import/fmi2CoSimulationSlave.cpp
        fmi4cpp/fmi2/import/fmi2ModelExchangeInstance.cpp
        fmi4cpp/fmi2/import/fmi2ModelExchangeSlave.cpp

        fmi4cpp/fmi2/xml/enums.cpp
        fmi4cpp/fmi2/xml/ModelDescription.cpp
        fmi4cpp/fmi2/xml/ModelDescriptionParser.cpp
        fmi4cpp/fmi2/xml/ModelVariables.cpp
        fmi4cpp/fmi2/xml/ScalarVariable.cpp
        fmi4cpp/fmi2/xml/TypedScalarVariable.cpp
        fmi4cpp/fmi2/xml/ScalarVariableAttribute.cpp

    )

set(FMI4CPP_LIB fmi4cpp)
add_library(${FMI4CPP_LIB} ${SOURCES})
add_library(FMI4cpp::${FMI4CPP_LIB} ALIAS fmi4cpp)

target_include_directories(${FMI4CPP_LIB}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    INTERFACE
        ${Boost_INCLUDE_DIRS}
    PRIVATE
        ${LIBZIP_INCLUDE_DIRS}
        ${CURRENT_CMAKE_SOURCE_DIR}/../src
)

target_link_libraries(${FMI4CPP_LIB}
    PRIVATE
        LIBZIP::LIBZIP
        spdlog::spdlog
)

if (FMI4CPP_WITH_CURL)
    target_include_directories(${FMI4CPP_LIB}
        PRIVATE
            ${CURL_INCLUDE_DIRS}
    )
    target_link_libraries(${FMI4CPP_LIB}
        PRIVATE
            ${CURL_LIBRARIES}
    )
endif()

if (UNIX)
    target_link_libraries(${FMI4CPP_LIB}
        PRIVATE
            dl stdc++fs pthread
    )
    if (FMI4CPP_USING_VCPKG)
        target_link_libraries(${FMI4CPP_LIB}
            PRIVATE
                ZLIB::ZLIB BZip2::BZip2
            )
    endif()
endif()

# Make sure that also downstream libraries that use fmi4cpp compile as C++17
target_compile_features(${FMI4CPP_LIB} PUBLIC cxx_std_17)

# Enable fmi4cpp debug messages for debug configurations
target_compile_definitions(${FMI4CPP_LIB} PRIVATE $<$<CONFIG:Debug>:FMI4CPP_LOG_LEVEL_DEBUG>)

if(FMI4CPP_WITH_ODEINT)
    target_compile_definitions(${FMI4CPP_LIB} PUBLIC FMI4CPP_WITH_ODEINT)
endif()

if(FMI4CPP_WITH_CURL)
    target_compile_definitions(${FMI4CPP_LIB} PUBLIC FMI4CPP_WITH_CURL)
endif()

if (FMI4CPP_LOG_LEVEL STREQUAL "OFF")
    target_compile_definitions(${FMI4CPP_LIB} PUBLIC FMI4CPP_LOG_LEVEL_OFF)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "INFO")
    target_compile_definitions(${FMI4CPP_LIB} PUBLIC FMI4CPP_LOG_LEVEL_INFO)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "DEBUG")
    target_compile_definitions(${FMI4CPP_LIB} PUBLIC FMI4CPP_LOG_LEVEL_DEBUG)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "DEBUG")
    target_compile_definitions(${FMI4CPP_LIB} PUBLIC FMI4CPP_LOG_LEVEL_TRACE)
else ()
    target_compile_definitions(${FMI4CPP_LIB} PUBLIC FMI4CPP_LOG_LEVEL_DEFAULT)
endif ()

##############################################
# Installation instructions
include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/FMI4cpp)

# Install target
install(TARGETS ${FMI4CPP_LIB}
    EXPORT ${FMI4CPP_LIB}-targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

#This is required so that the exported target has the name FMI4cpp and not fmi4cpp
set_target_properties(${FMI4CPP_LIB} PROPERTIES EXPORT_NAME ${PROJECT_NAME})

install(DIRECTORY ../include/
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
        REGEX ".*.(hpp|h)"
)

install(EXPORT ${FMI4CPP_LIB}-targets
    FILE
        ${PROJECT_NAME}Targets.cmake
    NAMESPACE
        ${PROJECT_NAME}::
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION
     ${PROJECT_VERSION}
    COMPATIBILITY
        AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/../cmake/${PROJECT_NAME}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION
        ${INSTALL_CONFIGDIR}
    )

#Install the config, configversion and custom find modules
install(FILES
        ${CMAKE_CURRENT_LIST_DIR}/../cmake/FindLIBZIP.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION
        ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
configure_file(${CMAKE_CURRENT_LIST_DIR}/../cmake/FindLIBZIP.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FindLIBZIP.cmake
    COPYONLY)


export(EXPORT ${FMI4CPP_LIB}-targets
    FILE
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
    NAMESPACE
        ${PROJECT_NAME}::
)

#Register package in user's package registry
export(PACKAGE ${PROJECT_NAME})


# Add uninstall logic (see https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake)
if (NOT TARGET uninstall)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/cmake_uninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
            IMMEDIATE @ONLY)

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
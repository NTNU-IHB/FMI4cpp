include(GNUInstallDirs)

SET(HEADERS

        ../include/fmi4cpp/tools/os_util.hpp
        ../include/fmi4cpp/tools/unzipper.hpp

        ../include/fmi4cpp/fmi2/fmi4cpp.hpp
        ../include/fmi4cpp/fmi2/fmi2Functions.h
        ../include/fmi4cpp/fmi2/fmi2FunctionTypes.h
        ../include/fmi4cpp/fmi2/fmi2TypesPlatform.h
        ../include/fmi4cpp/fmi2/enumsToString.hpp

        ../include/fmi4cpp/fmi2/import/FmuInstance.hpp
        ../include/fmi4cpp/fmi2/import/FmuSlave.hpp
        ../include/fmi4cpp/fmi2/import/InstanceBuilder.hpp

        ../include/fmi4cpp/fmi2/import/Fmu.hpp
        ../include/fmi4cpp/fmi2/import/FmiLibrary.hpp
        ../include/fmi4cpp/fmi2/import/CoSimulationLibrary.hpp
        ../include/fmi4cpp/fmi2/import/ModelExchangeLibrary.hpp
        ../include/fmi4cpp/fmi2/import/AbstractFmuInstance.hpp
        ../include/fmi4cpp/fmi2/import/CoSimulationSlave.hpp
        ../include/fmi4cpp/fmi2/import/CoSimulationSlaveBuilder.hpp
        ../include/fmi4cpp/fmi2/import/ModelExchangeInstance.hpp
        ../include/fmi4cpp/fmi2/import/ModelExchangeInstanceBuilder.hpp

        ../include/fmi4cpp/fmi2/xml/enums.hpp
        ../include/fmi4cpp/fmi2/xml/FmuTypeAttributes.hpp
        ../include/fmi4cpp/fmi2/xml/ModelDescription.hpp
        ../include/fmi4cpp/fmi2/xml/SpecificModelDescription.hpp
        ../include/fmi4cpp/fmi2/xml/ModelVariables.hpp
        ../include/fmi4cpp/fmi2/xml/ScalarVariable.hpp
        ../include/fmi4cpp/fmi2/xml/ScalarVariableAttributes.hpp
        ../include/fmi4cpp/fmi2/xml/DefaultExperiment.hpp
        ../include/fmi4cpp/fmi2/xml/SourceFiles.hpp
        ../include/fmi4cpp/fmi2/xml/ModelStructure.hpp)

SET(SOURCES

        fmi4cpp/fmi2/import/Fmu.cpp
        fmi4cpp/fmi2/import/FmiLibrary.cpp
        fmi4cpp/fmi2/import/CoSimulationLibrary.cpp
        fmi4cpp/fmi2/import/ModelExchangeLibrary.cpp
        fmi4cpp/fmi2/import/CoSimulationSlave.cpp
        fmi4cpp/fmi2/import/CoSimulationSlaveBuilder.cpp
        fmi4cpp/fmi2/import/ModelExchangeInstance.cpp
        fmi4cpp/fmi2/import/ModelExchangeInstanceBuilder.cpp

        fmi4cpp/fmi2/xml/enums.cpp
        fmi4cpp/fmi2/xml/FmuTypeAttributes.cpp
        fmi4cpp/fmi2/xml/ModelDescription.cpp
        fmi4cpp/fmi2/xml/SpecificModelDescription.cpp
        fmi4cpp/fmi2/xml/ModelVariables.cpp
        fmi4cpp/fmi2/xml/ScalarVariable.cpp
        fmi4cpp/fmi2/xml/ScalarVariableAttributes.cpp
        fmi4cpp/fmi2/xml/DefaultExperiment.cpp
        fmi4cpp/fmi2/xml/SourceFiles.cpp
        fmi4cpp/fmi2/xml/ModelStructure.cpp)

SET(FMI4CPP_LIB fmi4cpp)
add_library(${FMI4CPP_LIB} ${HEADERS} ${SOURCES})

target_include_directories(${FMI4CPP_LIB} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
                                         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${FMI4CPP_LIB} ${LIBZIP_LIBRARIES})
target_include_directories(${FMI4CPP_LIB} PUBLIC ${Boost_INCLUDE_DIRS} PRIVATE ${LIBZIP_INCLUDE_DIRS})

if (UNIX)
    target_link_libraries(${FMI4CPP_LIB} dl stdc++fs)
endif ()

# Make sure that also downstream libraries that use fmi4cpp compile as C++17
target_compile_features(${FMI4CPP_LIB} PUBLIC cxx_std_17)

#Enable fmi4cpp debug messages for debug configurations
target_compile_definitions(${FMI4CPP_LIB} PRIVATE $<$<CONFIG:Debug>:DEBUG_LOGGING_ENABLED>)

# Install target
install(TARGETS ${FMI4CPP_LIB}
        EXPORT ${PROJECT_NAME}
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")

# Install headers
install(DIRECTORY ../include/
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING REGEX ".*.(hpp|h)")

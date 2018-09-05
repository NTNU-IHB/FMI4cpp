
include_directories(../include)

SET(SOURCES
        fmicpp/fmi2/import/Fmu.cpp
        fmicpp/fmi2/import/FmiLibrary.cpp
        fmicpp/fmi2/import/CoSimulationLibrary.cpp
        fmicpp/fmi2/import/ModelExchangeLibrary.cpp
        fmicpp/fmi2/import/CoSimulationSlave.cpp
        fmicpp/fmi2/import/CoSimulationSlaveBuilder.cpp

        fmicpp/fmi2/xml/FmuData.cpp
        fmicpp/fmi2/xml/ModelDescription.cpp
        fmicpp/fmi2/xml/ModelVariables.cpp
        fmicpp/fmi2/xml/ScalarVariable.cpp
        fmicpp/fmi2/xml/ScalarVariableAttributes.cpp
        fmicpp/fmi2/xml/DefaultExperiment.cpp
        fmicpp/fmi2/xml/SourceFiles.cpp
        fmicpp/fmi2/xml/ModelStructure.cpp)

add_library(fmicpp ${SOURCES})
target_link_libraries(fmicpp Boost::system Boost::filesystem ${LIBZIP_LIBRARIES})
target_include_directories(fmicpp PRIVATE ${LIBZIP_INCLUDE_DIRS})
if (UNIX)
    target_link_libraries(fmicpp dl)
elseif(WIN32)
    target_link_libraries(fmicpp bcrypt) # required by boost::uuids
endif()
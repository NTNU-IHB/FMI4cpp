
include_directories(../include)

SET(HEADERS

        ../include/fmicpp/tools/os_util.hpp
        ../include/fmicpp/tools/unzipper.hpp

        ../include/fmicpp/fmi2/fmicpp.hpp
        ../include/fmicpp/fmi2/fmi2Functions.h
        ../include/fmicpp/fmi2/fmi2FunctionTypes.h
        ../include/fmicpp/fmi2/fmi2TypesPlatform.h

        ../include/fmicpp/fmi2/import/FmuInstance.hpp
        ../include/fmicpp/fmi2/import/FmuSlave.hpp

        ../include/fmicpp/fmi2/import/Fmu.hpp
        ../include/fmicpp/fmi2/import/FmiLibrary.hpp
        ../include/fmicpp/fmi2/import/CoSimulationLibrary.hpp
        ../include/fmicpp/fmi2/import/ModelExchangeLibrary.hpp
        ../include/fmicpp/fmi2/import/AbstractFmuInstance.hpp
        ../include/fmicpp/fmi2/import/CoSimulationSlave.hpp
        ../include/fmicpp/fmi2/import/CoSimulationSlaveBuilder.hpp
        ../include/fmicpp/fmi2/import/ModelExchangeInstance.hpp
        ../include/fmicpp/fmi2/import/ModelExchangeInstanceBuilder.hpp
        ../include/fmicpp/fmi2/xml/FmuTypeAttributes.hpp
        ../include/fmicpp/fmi2/xml/ModelDescription.hpp
        ../include/fmicpp/fmi2/xml/ModelVariables.hpp
        ../include/fmicpp/fmi2/xml/ScalarVariable.hpp
        ../include/fmicpp/fmi2/xml/ScalarVariableAttributes.hpp
        ../include/fmicpp/fmi2/xml/DefaultExperiment.hpp
        ../include/fmicpp/fmi2/xml/SourceFiles.hpp
        ../include/fmicpp/fmi2/xml/ModelStructure.hpp

)

SET(SOURCES
        fmicpp/fmi2/import/Fmu.cpp
        fmicpp/fmi2/import/FmiLibrary.cpp
        fmicpp/fmi2/import/CoSimulationLibrary.cpp
        fmicpp/fmi2/import/ModelExchangeLibrary.cpp
        fmicpp/fmi2/import/CoSimulationSlave.cpp
        fmicpp/fmi2/import/CoSimulationSlaveBuilder.cpp
        fmicpp/fmi2/import/ModelExchangeInstance.cpp
        fmicpp/fmi2/import/ModelExchangeInstanceBuilder.cpp

        fmicpp/fmi2/xml/FmuTypeAttributes.cpp
        fmicpp/fmi2/xml/ModelDescription.cpp
        fmicpp/fmi2/xml/ModelVariables.cpp
        fmicpp/fmi2/xml/ScalarVariable.cpp
        fmicpp/fmi2/xml/ScalarVariableAttributes.cpp
        fmicpp/fmi2/xml/DefaultExperiment.cpp
        fmicpp/fmi2/xml/SourceFiles.cpp
        fmicpp/fmi2/xml/ModelStructure.cpp)

add_library(fmicpp ${HEADERS} ${SOURCES})
target_link_libraries(fmicpp Boost::system Boost::filesystem ${LIBZIP_LIBRARIES})
target_include_directories(fmicpp PRIVATE ${LIBZIP_INCLUDE_DIRS})
if (UNIX)
    target_link_libraries(fmicpp dl)
elseif(WIN32)
    target_link_libraries(fmicpp bcrypt) # required by boost::uuids
endif()
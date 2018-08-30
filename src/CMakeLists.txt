
include_directories(../include)

SET(HEADERS

        ../include/fmicpp/fmi2/fmicpp.hpp

        ../include/fmicpp/fmi2/import/fmi2Functions.h
        ../include/fmicpp/fmi2/import/fmi2FunctionTypes.h
        ../include/fmicpp/fmi2/import/fmi2TypesPlatform.h
        ../include/fmicpp/fmi2/import/FmiLibrary.hpp
        ../include/fmicpp/fmi2/import/Fmu.hpp
        ../include/fmicpp/fmi2/import/FmuInstance.hpp
        ../include/fmicpp/fmi2/import/FmuSlave.hpp

        ../include/fmicpp/fmi2/xml/ModelDescription.hpp
        ../include/fmicpp/fmi2/xml/ModelVariables.hpp
        ../include/fmicpp/fmi2/xml/ScalarVariable.hpp
        ../include/fmicpp/fmi2/xml/DefaultExperiment.hpp
        ../include/fmicpp/fmi2/xml/SourceFiles.hpp
        ../include/fmicpp/fmi2/xml/ModelStructure.hpp

)

SET(SOURCES
        fmicpp/fmi2/import/Fmu.cpp
        fmicpp/fmi2/import/FmiLibrary.cpp

        fmicpp/fmi2/xml/ModelDescription.cpp
        fmicpp/fmi2/xml/ModelVariables.cpp
        fmicpp/fmi2/xml/ScalarVariable.cpp
        fmicpp/fmi2/xml/DefaultExperiment.cpp
        fmicpp/fmi2/xml/SourceFiles.cpp
        fmicpp/fmi2/xml/ModelStructure.cpp)

add_library(fmicpp ${HEADERS} ${SOURCES})
target_link_libraries(fmicpp Boost::system Boost::filesystem)
if (UNIX)
    target_link_libraries(fmicpp dl)
endif()
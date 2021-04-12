set(publicHeaderDir ${CMAKE_SOURCE_DIR}/include)

set(publicHeaders

    "fmi4cpp/fmi4cpp.hpp"
    "fmi4cpp/status.hpp"
    "fmi4cpp/types.hpp"

    "fmi4cpp/fmu_base.hpp"
    "fmi4cpp/fmu_slave.hpp"
    "fmi4cpp/fmu_resource.hpp"
    "fmi4cpp/fmu_instance.hpp"
    "fmi4cpp/fmu_instance_base.hpp"
    "fmi4cpp/fmu_variable_accessor.hpp"

    "fmi4cpp/dll_handle.hpp"
    "fmi4cpp/fs_portability.hpp"

    "fmi4cpp/fmi2/fmi2.hpp"
    "fmi4cpp/fmi2/fmu.hpp"
    "fmi4cpp/fmi2/fmi2_library.hpp"

    "fmi4cpp/fmi2/fmi2Functions.h"
    "fmi4cpp/fmi2/fmi2FunctionTypes.h"
    "fmi4cpp/fmi2/fmi2TypesPlatform.h"

    "fmi4cpp/fmi2/cs_fmu.hpp"
    "fmi4cpp/fmi2/cs_library.hpp"
    "fmi4cpp/fmi2/cs_slave.hpp"

    "fmi4cpp/fmi2/me_fmu.hpp"
    "fmi4cpp/fmi2/me_library.hpp"
    "fmi4cpp/fmi2/me_instance.hpp"

    "fmi4cpp/fmi2/xml/enums.hpp"
    "fmi4cpp/fmi2/xml/source_files.hpp"

    "fmi4cpp/fmi2/xml/default_experiment.hpp"
    "fmi4cpp/fmi2/xml/fmu_attributes.hpp"
    "fmi4cpp/fmi2/xml/model_structure.hpp"
    "fmi4cpp/fmi2/xml/model_description.hpp"
    "fmi4cpp/fmi2/xml/specific_model_description"
    "fmi4cpp/fmi2/xml/me_model_description.hpp"
    "fmi4cpp/fmi2/xml/cs_model_description.hpp"
    "fmi4cpp/fmi2/xml/model_description_parser.hpp"

    "fmi4cpp/fmi2/xml/model_variables.hpp"
    "fmi4cpp/fmi2/xml/scalar_variable.hpp"
    "fmi4cpp/fmi2/xml/typed_scalar_variable.hpp"

)

set(privateHeaders

    "fmi4cpp/mlog.hpp"
    "fmi4cpp/library_helper.hpp"
    "fmi4cpp/optional_converter.hpp"

    "fmi4cpp/fmi2/status_converter.hpp"

    "fmi4cpp/tools/os_util.hpp"
    "fmi4cpp/tools/time_util.hpp"
    "fmi4cpp/tools/unzipper.hpp"

)

set(sources

    "fmi4cpp/fmu_resource.cpp"

    "fmi4cpp/fmi2/fmu.cpp"
    "fmi4cpp/fmi2/fmi2_library.cpp"

    "fmi4cpp/fmi2/cs_fmu.cpp"
    "fmi4cpp/fmi2/me_fmu.cpp"
    "fmi4cpp/fmi2/cs_library.cpp"
    "fmi4cpp/fmi2/cs_slave.cpp"
    "fmi4cpp/fmi2/me_library.cpp"
    "fmi4cpp/fmi2/me_instance.cpp"

    "fmi4cpp/fmi2/xml/enums.cpp"
    "fmi4cpp/fmi2/xml/model_description.cpp"
    "fmi4cpp/fmi2/xml/model_description_parser.cpp"
    "fmi4cpp/fmi2/xml/model_variables.cpp"
    "fmi4cpp/fmi2/xml/scalar_variable.cpp"

)

set(publicHeadersFull)
foreach(header IN LISTS publicHeaders)
    list(APPEND publicHeadersFull "${publicHeaderDir}/${header}")
endforeach()

add_library(fmi4cpp ${publicHeadersFull} ${privateHeaders} ${sources})
add_library(fmi4cpp::fmi4cpp ALIAS fmi4cpp)
target_compile_definitions(fmi4cpp PRIVATE "BOOST_ALL_DYN_LINK=1" "BOOST_ALL_NO_LIB=1" "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1")
target_compile_features(fmi4cpp PUBLIC "cxx_std_17")

target_include_directories(fmi4cpp
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(fmi4cpp
    PRIVATE
        libzip::libzip
        Boost::boost
)

if(WIN32)
    target_link_libraries(fmi4cpp PRIVATE "Bcrypt")
elseif(UNIX)
    target_link_libraries(fmi4cpp PRIVATE stdc++fs dl)
endif()


if (FMI4CPP_LOG_LEVEL STREQUAL "OFF")
    target_compile_definitions(fmi4cpp PRIVATE MLOG_LEVEL_OFF)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "TRACE")
    target_compile_definitions(fmi4cpp PRIVATE MLOG_LEVEL_TRACE)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "DEBUG")
    target_compile_definitions(fmi4cpp PRIVATE MLOG_LEVEL_DEBUG)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "INFO")
    target_compile_definitions(fmi4cpp PRIVATE MLOG_LEVEL_INFO)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "WARN")
    target_compile_definitions(fmi4cpp PRIVATE MLOG_LEVEL_WARN)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "ERROR")
    target_compile_definitions(fmi4cpp PRIVATE MLOG_LEVEL_ERROR)
elseif (FMI4CPP_LOG_LEVEL STREQUAL "FATAL")
    target_compile_definitions(fmi4cpp PRIVATE MLOG_LEVEL_FATAL)
else ()
    target_compile_definitions(fmi4cpp PRIVATE $<$<CONFIG:Debug>:MLOG_LEVEL_DEBUG>)
endif ()

install(
    TARGETS fmi4cpp
    EXPORT "${FMI4CPP_EXPORT_TARGET}"
    ${FMI4CPP_INSTALL_DESTINATIONS}
)
install(
    DIRECTORY "${publicHeaderDir}/fmi4cpp"
    DESTINATION "${FMI4CPP_HEADER_INSTALL_DIR}"
)


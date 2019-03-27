
set(publicHeaderDir ${CMAKE_CURRENT_SOURCE_DIR}/../include/fmi4cpp)
set(privateHeaderDir "${CMAKE_CURRENT_SOURCE_DIR}/fmi4cpp")

set(publicHeaders

    fmi4cpp.hpp
    status.hpp
    types.hpp

    fmu_base.hpp
    fmu_slave.hpp
    fmu_resource.hpp
    fmu_instance.hpp
    fmu_instance_base.hpp
    fmu_variable_accessor.hpp

    driver/driver_options.hpp
    driver/error_types.hpp
    driver/fmu_driver.hpp

    solver/solver.hpp

    fmi2/fmi2.hpp
    fmi2/fmu.hpp
    fmi2/fmi2_library.hpp

    fmi2/fmi2Functions.h
    fmi2/fmi2FunctionTypes.h
    fmi2/fmi2TypesPlatform.h

    fmi2/cs_fmu.hpp
    fmi2/cs_library.hpp
    fmi2/cs_slave.hpp

    fmi2/me_fmu.hpp
    fmi2/me_library.hpp
    fmi2/me_slave.hpp
    fmi2/me_instance.hpp

    fmi2/xml/enums.hpp
    fmi2/xml/source_files.hpp

    fmi2/xml/default_experiment.hpp
    fmi2/xml/fmu_attributes.hpp
    fmi2/xml/model_structure.hpp
    fmi2/xml/model_description.hpp
    fmi2/xml/specific_model_description
    fmi2/xml/me_model_description.hpp
    fmi2/xml/cs_model_description.hpp
    fmi2/xml/model_description_parser.hpp

    fmi2/xml/model_variables.hpp
    fmi2/xml/scalar_variable.hpp
    fmi2/xml/typed_scalar_variable.hpp

)

set(privateHeaders

    mlog.hpp

    fmi2/status_converter.hpp
    fmi2/library_helper.hpp

    tools/os_util.hpp
    tools/time_util.hpp
    tools/unzipper.hpp
    tools/optional_converter.hpp
)

set(sources

    fmu_resource.cpp

    fmi2/fmu.cpp
    fmi2/fmi2_library.cpp

    fmi2/cs_fmu.cpp
    fmi2/me_fmu.cpp
    fmi2/cs_library.cpp
    fmi2/cs_slave.cpp
    fmi2/me_library.cpp

    fmi2/me_instance.cpp
    fmi2/me_slave.cpp

    fmi2/xml/enums.cpp
    fmi2/xml/model_description.cpp
    fmi2/xml/model_description_parser.cpp
    fmi2/xml/model_variables.cpp
    fmi2/xml/scalar_variable.cpp

    driver/fmu_driver.cpp

)

set(publicHeadersFull)
foreach(header IN LISTS publicHeaders)
    list(APPEND publicHeadersFull "${publicHeaderDir}/${header}")
endforeach()
set(privateHeadersFull)
foreach(header IN LISTS privateHeaders)
    list(APPEND privateHeadersFull "${privateHeaderDir}/${header}")
endforeach()
set(allSources ${publicHeadersFull} ${privateHeadersFull} ${sources})

add_library(fmi4cpp ${allSources})
add_library(fmi4cpp::fmi4cpp ALIAS fmi4cpp)
target_compile_definitions(fmi4cpp PUBLIC "BOOST_ALL_DYN_LINK=1" "BOOST_ALL_NO_LIB=1" "BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1")
target_compile_features(fmi4cpp PUBLIC "cxx_std_17")

if(FMI4CPP_WITH_ODEINT)
    list(APPEND publicHeaders solver/odeint_solver.hpp)
    target_compile_definitions(fmi4cpp PUBLIC FMI4CPP_WITH_ODEINT)
endif()

if(FMI4CPP_WITH_CURL)
    target_compile_definitions(fmi4cpp PRIVATE FMI4CPP_WITH_CURL)
endif()

target_include_directories(fmi4cpp
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        ${Boost_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(fmi4cpp PRIVATE libzip::libzip)

if (FMI4CPP_WITH_CURL)
    target_link_libraries(fmi4cpp PRIVATE curl::curl)
endif()

if (UNIX)
    target_link_libraries(fmi4cpp PRIVATE dl stdc++fs pthread)
endif()

install(
    TARGETS fmi4cpp
    EXPORT "${FMI4CPP_EXPORT_TARGET}"
    ${FMI4CPP_INSTALL_DESTINATIONS}
)
install(
    DIRECTORY "${publicHeaderDir}"
    DESTINATION "${FMI4CPP_HEADER_INSTALL_DIR}"
)

include(set-log-level)

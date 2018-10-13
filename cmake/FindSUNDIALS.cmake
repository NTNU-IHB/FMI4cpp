# Find Sundials
#
# Find the native libzip headers and libraries.
#
# This will define the following variables:
#
#   SUNDIALS_INCLUDE_DIRS   - Sundials include dirs
#   SUNDIALS_LIBRARIES      - List of libraries to link when using Sundials.
#   SUNDIALS_FOUND          - True if Sundials found.
#
# and the following imported targets
#
#   SUNDIALS::SUNDIALS
#
# @author Lars Ivar Hatledal

find_path(SUNDIALS_INCLUDE_DIR NAMES sundials/sundials_config.h)
mark_as_advanced(SUNDIALS_INCLUDE_DIR)

#cvode
find_library(SUNDIALS_CVODE_LIBRARY NAMES sundials_cvode)
mark_as_advanced(SUNDIALS_CVODE_LIBRARY)

#cvodes
find_library(SUNDIALS_CVODES_LIBRARY NAMES sundials_cvodes)
mark_as_advanced(SUNDIALS_CVODES_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SUNDIALS
        REQUIRED_VARS SUNDIALS_CVODE_LIBRARY SUNDIALS_CVODES_LIBRARY SUNDIALS_INCLUDE_DIR)

if (SUNDIALS_FOUND)

    set(SUNDIALS_INCLUDE_DIRS ${SUNDIALS_INCLUDE_DIR})

    if (NOT SUNDIALS_LIBRARIES)
        set(SUNDIALS_LIBRARIES ${SUNDIALS_CVODE_LIBRARY} ${SUNDIALS_CVODES_LIBRARY})
    endif()

    if (NOT TARGET SUNDIALS::SUNDIALS)
        add_library(SUNDIALS::SUNDIALS UNKNOWN IMPORTED)
        set_target_properties(SUNDIALS::SUNDIALS PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${SUNDIALS_INCLUDE_DIR}")
        set_property(TARGET SUNDIALS::SUNDIALS APPEND PROPERTY
                IMPORTED_LOCATION "${SUNDIALS_LIBRARIES}")
    endif()

endif()
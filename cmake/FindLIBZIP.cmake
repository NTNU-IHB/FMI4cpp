# Find libzip
#
# Find the native libzip headers and libraries.
#
# This will define the following variables:
#
#   LIBZIP_INCLUDE_DIRS   - libzip include dirs
#   LIBZIP_LIBRARIES      - List of libraries to link when using libzip.
#   LIBZIP_FOUND          - True if libzip found.
#
# and the following imported targets
#
#   LIBZIP::LIBZIP
#
# @author Lars Ivar Hatledal

find_package(ZLIB REQUIRED)
find_package(BZip2)

find_path(LIBZIP_INCLUDE_DIR NAMES zip.h)
mark_as_advanced(LIBZIP_INCLUDE_DIR)

find_library(LIBZIP_LIBRARY NAMES zip)
mark_as_advanced(LIBZIP_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBZIP
        REQUIRED_VARS LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR)

if (LIBZIP_FOUND)

    set(LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDE_DIR})

    if (NOT LIBZIP_LIBRARIES)
        set(LIBZIP_LIBRARIES ${LIBZIP_LIBRARY})
    endif()

    if (NOT TARGET LIBZIP::LIBZIP)
        add_library( LIBZIP::LIBZIP UNKNOWN IMPORTED)
        set_property(TARGET  LIBZIP::LIBZIP
                APPEND
                PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${LIBZIP_INCLUDE_DIR}")
        set_property(TARGET  LIBZIP::LIBZIP
                APPEND
                PROPERTY IMPORTED_LOCATION "${LIBZIP_LIBRARY}")

        set(INTERFACE_LINK_LIBRARIES ZLIB::ZLIB)
        if (BZip2_FOUND)
            list(APPEND INTERFACE_LINK_LIBRARIES BZip2::BZip2)
        endif()
        set_property(TARGET  LIBZIP::LIBZIP
                APPEND
                PROPERTY INTERFACE_LINK_LIBRARIES
                ${INTERFACE_LINK_LIBRARIES})

    endif()

endif()
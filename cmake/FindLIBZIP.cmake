
# - CMake script for locating libzip
#
# If the script succeeds, it will create an IMPORTED target named
# "libzip::libzip", plus set the following variables:
#
#    LIBZIP_FOUND           - If the library was found
#    LIBZIP_INCLUDE_DIRS    - The directory that contains the header files.
#    LIBZIP_LIBRARIES       - Contains "libzip::libzip"
#    LIBZIP_LIBRARY         - Path to static/import library file.
#

find_package(BZip2 QUIET) # optional package - used when building using conan
find_package(OpenSSL COMPONENTS Crypto SSL QUIET) # optional package - used when building using conan on linux
find_package(ZLIB QUIET) # optional package - used when building using conan
find_package(LibLZMA QUIET) # optional package - used when building using conan

# Find static library, and use its path prefix to provide a HINTS option to the
# other find_*() commands.
find_library (LIBZIP_LIBRARY "zip"
        PATHS ${LIBZIP_DIR} $ENV{LIBZIP_DIR}
        PATH_SUFFIXES "lib")
mark_as_advanced (LIBZIP_LIBRARY)
unset (_LIBZIP_hints)
if (LIBZIP_LIBRARY)
    get_filename_component (_LIBZIP_prefix "${LIBZIP_LIBRARY}" PATH)
    get_filename_component (_LIBZIP_prefix "${_LIBZIP_prefix}" PATH)
    set (_LIBZIP_hints "HINTS" "${_LIBZIP_prefix}")
    unset (_LIBZIP_prefix)
endif ()

# Find header files and, on Windows, the DLL
find_path (LIBZIP_INCLUDE_DIR_zip "zip.h"
        ${_LIBZIP_hints}
        PATHS ${LIBZIP_DIR} $ENV{LIBZIP_DIR}
        PATH_SUFFIXES "include")
find_path (LIBZIP_INCLUDE_DIR_zipconf "zipconf.h"
        ${_LIBZIP_hints}
        PATHS ${LIBZIP_DIR} $ENV{LIBZIP_DIR}
        PATH_SUFFIXES "include" "lib/libzip/include")
set (LIBZIP_INCLUDE_DIRS
        "${LIBZIP_INCLUDE_DIR_zip}" "${LIBZIP_INCLUDE_DIR_zipconf}"
        )

mark_as_advanced (LIBZIP_INCLUDE_DIRS)

if (WIN32)
    find_file (LIBZIP_DLL NAMES "zip.dll" "libzip.dll"
            ${_LIBZIP_hints}
            PATHS ${LIBZIP_DIR} $ENV{LIBZIP_DIR}
            PATH_SUFFIXES "bin" "lib"
            NO_CMAKE_PATH NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
    mark_as_advanced (LIBZIP_DLL)
endif ()
unset (_LIBZIP_hints)

# Create the IMPORTED targets.
if (LIBZIP_LIBRARY)
    add_library ("libzip::libzip" SHARED IMPORTED)
    set_target_properties ("libzip::libzip" PROPERTIES
            IMPORTED_LINK_INTERFACE_LANGUAGES "C"
            IMPORTED_LOCATION "${LIBZIP_LIBRARY}"
            INTERFACE_COMPILE_DEFINITIONS "LIBZIP_STATIC_LIB_ONLY"
            INTERFACE_INCLUDE_DIRECTORIES "${LIBZIP_INCLUDE_DIRS}")
    if (WIN32)
        set_target_properties ("libzip::libzip" PROPERTIES
                IMPORTED_IMPLIB "${LIBZIP_LIBRARY}"
                IMPORTED_LOCATION "${LIBZIP_DLL}")
    else () # not WIN32
        set_target_properties ("libzip::libzip" PROPERTIES
                IMPORTED_LOCATION "${LIBZIP_LIBRARY}")
    endif ()

    set (LIBZIP_LIBRARIES "libzip::libzip")

    ##### required by conan when linking libzip statically  ####
    set(LIBZIP_interfaceLinkLibraries)
    if (ZLIB_FOUND)
        list(APPEND LIBZIP_interfaceLinkLibraries ZLIB::ZLIB)
    endif()
    if (LIBLZMA_FOUND)
        list(APPEND LIBZIP_interfaceLinkLibraries LibLZMA::LibLZMA)
    endif()
    if (BZip2_FOUND)
        list(APPEND LIBZIP_interfaceLinkLibraries BZip2::BZip2)
    endif()
    if (OpenSSL_FOUND)
        list(APPEND LIBZIP_interfaceLinkLibraries OpenSSL::SSL OpenSSL::Crypto)
    endif()
    if (BZip2_FOUND OR OpenSSL_FOUND)
        set_property(TARGET libzip::libzip
                APPEND
                PROPERTY INTERFACE_LINK_LIBRARIES
                ${LIBZIP_interfaceLinkLibraries})
    endif()
    ###############################################################
endif ()

# Debug print-out.
if (LIBZIP_PRINT_VARS)
    message ("LIBZIP find script variables:")
    message ("  LIBZIP_INCLUDE_DIRS   = ${LIBZIP_INCLUDE_DIRS}")
    message ("  LIBZIP_LIBRARIES      = ${LIBZIP_LIBRARIES}")
    message ("  LIBZIP_DLL            = ${LIBZIP_DLL}")
    message ("  LIBZIP_LIBRARY        = ${LIBZIP_LIBRARY}")
endif ()

# Standard find_package stuff.
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (LIBZIP DEFAULT_MSG LIBZIP_LIBRARIES LIBZIP_INCLUDE_DIRS)
# Find libzip
#
# Find the native libzip headers and libraries.
#
#   LIBZIP_INCLUDE_DIRS   - libzip include dirs
#   LIBZIP_LIBRARIES      - List of libraries to link when using libzip.
#   LIBZIP_FOUND          - True if libzip found.

find_path(LIBZIP_INCLUDE_DIR NAMES zip.h)
mark_as_advanced(LIBZIP_INCLUDE_DIR)

find_library(LIBZIP_LIBRARY NAMES zip)
mark_as_advanced(LIBZIP_LIBRARY)

include(FindPackageHandleStandardArgs)

if (UNIX)

    find_package(ZLIB REQUIRED)
    find_package(BZip2 REQUIRED)
    find_package(OpenSSL REQUIRED)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBZIP
            REQUIRED_VARS LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR)

    set(LIBZIP_LIBRARIES ${LIBZIP_LIBRARY} ${ZLIB_LIBRARIES} ${BZIP2_LIBRARIES} ${OPENSSL_LIBRARIES})

else ()

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBZIP
            REQUIRED_VARS LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR)

    set(LIBZIP_LIBRARIES ${LIBZIP_LIBRARY})

endif ()

set(LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDE_DIR})


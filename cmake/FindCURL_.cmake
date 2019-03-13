
find_package(OpenSSL COMPONENTS Crypto SSL)
find_package(CURL REQUIRED)

if (NOT TARGET CURL::CURL)
    add_library(CURL::CURL UNKNOWN IMPORTED)
    set_property(TARGET CURL::CURL
            APPEND
            PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CURL_INCLUDE_DIR}")
    set_property(TARGET CURL::CURL
            APPEND
            PROPERTY IMPORTED_LOCATION "${CURL_LIBRARY}")
    if (OpenSSL_FOUND)
        set(INTERFACE_LINK_LIBRARIES OpenSSL::SSL OpenSSL::Crypto)
    endif()
    set_property(TARGET CURL::CURL
            APPEND
            PROPERTY INTERFACE_LINK_LIBRARIES
            ${INTERFACE_LINK_LIBRARIES})
endif()
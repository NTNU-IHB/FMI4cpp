
#ifndef FMI4CPP_OS_UTIL_HPP
#define FMI4CPP_OS_UTIL_HPP

#include <string>

namespace
{

std::string get_os()
{
#if _WIN32 || _WIN64
#    if _WIN64
    return "win64";
#    else
    return "win32";
#    endif
#elif __linux__
    return "linux64";
#endif
}

std::string get_shared_library_extension()
{
#if _WIN32
    return ".dll";
#elif __linux__
    return ".so";
#endif
}
} // namespace

#endif //FMI4CPP_OS_UTIL_HPP

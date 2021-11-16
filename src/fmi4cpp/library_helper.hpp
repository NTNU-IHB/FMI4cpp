
#ifndef FMI4CPP_LIBRARYHELPER_HPP
#define FMI4CPP_LIBRARYHELPER_HPP

#include <fmi4cpp/dll_handle.hpp>

#include <sstream>

namespace
{

DLL_HANDLE load_library(const std::string& libName)
{
#ifdef WIN32
    return LoadLibrary(libName.c_str());
#else
    return dlopen(libName.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
}

template<class T>
T load_function(DLL_HANDLE handle, const char* function_name)
{
#ifdef WIN32
    return (T)GetProcAddress(handle, function_name);
#else
    return (T)dlsym(handle, function_name);
#endif
}

bool free_library(DLL_HANDLE handle)
{
#ifdef WIN32
    return static_cast<bool>(FreeLibrary(handle));
#else
    return (dlclose(handle) == 0);
#endif
}

std::string getLastError()
{
#ifdef WIN32
    std::ostringstream os;
    os << GetLastError();
    return os.str();
#else
    return dlerror();
#endif
}

} // namespace

#endif //FMI4CPP_LIBRARYHELPER_HPP

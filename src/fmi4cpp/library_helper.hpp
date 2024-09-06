
#ifndef FMI4CPP_LIBRARYHELPER_HPP
#define FMI4CPP_LIBRARYHELPER_HPP

#include <fmi4cpp/dll_handle.hpp>
#include <fmi4cpp/fs_portability.hpp>
#include <sstream>

namespace
{

DLL_HANDLE load_library(const std::string& libName)
{
    std::string dllDirectory;

#ifdef _WIN32
    fmi4cpp::fs::path path(libName);
    if (path.has_parent_path()) {
        dllDirectory = path.parent_path().string();
    }

    DLL_DIRECTORY_COOKIE dllDirectoryCookie = nullptr;
    if (!dllDirectory.empty()) {
        std::wstring wDllDirectory(dllDirectory.begin(), dllDirectory.end());
        dllDirectoryCookie = AddDllDirectory(wDllDirectory.c_str());
    }

    DLL_HANDLE handle = LoadLibrary(libName.c_str());

    if (dllDirectoryCookie) {
        RemoveDllDirectory(dllDirectoryCookie);
    }

    return handle;

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

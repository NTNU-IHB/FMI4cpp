//
// Created by laht on 24.09.18.
//

#ifndef FMI4CPP_FMILIBRARYHELPER_HPP
#define FMI4CPP_FMILIBRARYHELPER_HPP

#include <fmi4cpp/fmi2/import/FmiLibrary.hpp>

namespace {

    DLL_HANDLE loadLibrary(const std::string libName) {
#ifdef WIN32
        return LoadLibrary(libName.c_str());
#else
        return dlopen(libName.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
    }

    template<class T>
    T loadFunction(DLL_HANDLE handle, const char *function_name) {
#ifdef WIN32
        return (T) GetProcAddress(handle, function_name);
#else
        return (T) dlsym(handle, function_name);
#endif
    }

}

#endif //FMI4CPP_FMILIBRARYHELPER_HPP

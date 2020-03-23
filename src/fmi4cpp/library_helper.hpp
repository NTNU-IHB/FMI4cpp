/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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

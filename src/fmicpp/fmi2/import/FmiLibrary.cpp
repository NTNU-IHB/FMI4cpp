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

#include <iostream>
#include <fmicpp/fmi2/import/FmiLibrary.hpp>

using namespace std;
using namespace fmicpp::fmi2::import;

FmiLibrary::FmiLibrary(string lib_name) {
#ifdef WIN32
    handle_ = LoadLibrary(lib_name.c_str());
#else
    handle_ = dlopen(lib_name.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif

    if (!handle_) {
        string msg = "Unable to load dynamic library '" + lib_name + "'!";
        throw runtime_error(msg);
    }

}

const char *FmiLibrary::getVersion() {
    typedef const char* (*fun_t)(void);
    return loadFunction<fun_t >("fmi2GetVersion")();
}

const char *FmiLibrary::getTypesPlatform() {
    typedef const char* (*fun_t)(void);
    return loadFunction<fun_t >("fmi2GetTypesPlatform")();
}

template<class T>
T FmiLibrary::loadFunction(const char *function_name) {
#ifdef WIN32
    return (T) GetProcAddress(handle_, function_name);
#else
    return (T) dlsym(handle_, function_name);
#endif
}

FmiLibrary::~FmiLibrary() {

   if (handle_) {
#ifdef WIN32
       FreeLibrary(handle_)
#else
       if (dlclose(handle_) != 0) {
           cout << dlerror() << endl;
       }
#endif
       handle_ = nullptr;
   }

}

/*
 * The MIT License
 *
 * Copyright 2017-2020 Norwegian University of Technology
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

#ifndef FMI4CPP_DLL_HANDLE_HPP
#define FMI4CPP_DLL_HANDLE_HPP

#if defined(_MSC_VER) || defined(WIN32) || defined(__MINGW32__)
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    define DLL_HANDLE HMODULE
#else
#    define DLL_HANDLE void*
#    include <dlfcn.h>
#endif

#ifdef WIN32
#    define function_ptr FARPROC
#else
typedef void* function_ptr;
#endif

#endif //FMI4CPP_DLL_HANDLE_HPP

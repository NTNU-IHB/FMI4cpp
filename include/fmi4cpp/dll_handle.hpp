
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

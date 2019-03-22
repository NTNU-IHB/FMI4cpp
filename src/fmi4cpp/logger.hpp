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

#ifndef FMI4CPP_LOGGER_HPP
#define FMI4CPP_LOGGER_HPP


#include <iostream>
#include <string>

namespace {

    enum fmi4cpp_log_level {

        All = 1,
        Debug = 2,
        Info = 3,
        Warn = 4,
        Error = 5

    };

#ifndef FMI4CPP_LOG_LEVEL_OFF

    #if FMI4CPP_LOG_LEVEL_ALL
        fmi4cpp_log_level current_log_level = All;
    #elif FMI4CPP_LOG_LEVEL_DEBUG
        fmi4cpp_log_level current_log_level = Debug;
    #elif FMI4CPP_LOG_LEVEL_INFO
        fmi4cpp_log_level current_log_level = Info;
    #elif FMI4CPP_LOG_LEVEL_WARN
        fmi4cpp_log_level current_log_level = Warn;
    #elif FMI4CPP_LOG_LEVEL_ERROR
        fmi4cpp_log_level current_log_level = Error;
    #elif FMI4CPP_LOG_LEVEL_DEFAULT
        fmi4cpp_log_level current_log_level = Info;
    #endif

#define FMI4CPP_DEBUG( msg ) _FMI4CPP_LOG_(msg, Debug)
#define FMI4CPP_INFO( msg ) _FMI4CPP_LOG_(msg, Info)
#define FMI4CPP_WARN( msg ) _FMI4CPP_LOG_ (msg, Warn)
#define FMI4CPP_ERROR( msg ) _FMI4CPP_LOG_ (msg, Error)

#define _FMI4CPP_LOG_( msg, level ) {\
    if (level >= current_log_level) {\
        __FMI4CPP_LOG__( msg, level );\
    }\
}

#define __FMI4CPP_LOG__( msg, level) {\
    if ( level > Warn) {\
        std::cerr << __FILE__ << ":" << __LINE__ << ": " << msg << std::endl;\
    } else {\
        std::cout << __FILE__ << ":" << __LINE__ << ": " << msg << std::endl;\
    }\
}


#else

#define FMI4CPP_DEBUG( msg )
#define FMI4CPP_INFO( msg )
#define FMI4CPP_WARN( msg )
#define FMI4CPP_ERROR( msg )

#endif


}


#endif //FMI4CPP_LOGGER_HPP

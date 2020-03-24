/*
 * The MIT License
 *
 * Copyright 2017-2019 Norwegian University of Technology
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

#ifndef MLOG_HPP
#define MLOG_HPP

#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

namespace
{

#ifndef LOG_LEVEL_OFF

enum mlog_level
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Fatal = 5
};

const char* to_string(mlog_level lvl)
{
    switch (lvl) {
        case Trace: return "Trace";
        case Debug: return "Debug";
        case Info: return "Info";
        case Warn: return "Warn";
        case Error: return "Error";
        case Fatal: return "Fatal";
        default: return nullptr;
    }
}

#    if MLOG_LEVEL_TRACE
mlog_level M_LOG_LEVEL = Trace;
#    elif MLOG_LEVEL_DEBUG
mlog_level M_LOG_LEVEL = Debug;
#    elif MLOG_LEVEL_INFO
mlog_level M_LOG_LEVEL = Info;
#    elif MLOG_LEVEL_WARN
mlog_level M_LOG_LEVEL = Warn;
#    elif MLOG_LEVEL_ERROR
mlog_level M_LOG_LEVEL = Error;
#    elif MLOG_LEVEL_FATAL
mlog_level M_LOG_LEVEL = Fatal;
#    else
mlog_level M_LOG_LEVEL = Info;
#    endif

#    define MLOG_TRACE(msg) _MLOG_(msg, Trace)
#    define MLOG_DEBUG(msg) _MLOG_(msg, Debug)
#    define MLOG_INFO(msg) _MLOG_(msg, Info)
#    define MLOG_WARN(msg) _MLOG_(msg, Warn)
#    define MLOG_ERROR(msg) _MLOG_(msg, Error)
#    define MLOG_FATAL(msg) _MLOG_(msg, Fatal)

#    define _MLOG_(msg, level)                                                                                                                    \
        {                                                                                                                                         \
            if (level >= M_LOG_LEVEL) {                                                                                                           \
                auto time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());                                           \
                __MLOG__("[" << to_string(level) << "] [" << std::ctime(&time_now) << "] " << __FILE__ << ":" << __LINE__ << ": " << msg, level); \
            }                                                                                                                                     \
        }

#    define __MLOG__(msg, level)               \
        {                                      \
            if (level > Warn) {                \
                std::cerr << msg << std::endl; \
            } else {                           \
                std::cout << msg << std::endl; \
            }                                  \
        }

#else

#    define MLOG_TRACE(msg)
#    define MLOG_DEBUG(msg)
#    define MLOG_INFO(msg)
#    define MLOG_WARN(msg)
#    define MLOG_ERROR(msg)
#    define MLOG_FATAL(msg)

#endif

} // namespace

#endif //MLOG_HPP

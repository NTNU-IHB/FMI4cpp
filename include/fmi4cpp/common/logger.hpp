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

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>

namespace {

    inline std::shared_ptr<spdlog::logger> getOrCreateLogger() {

        std::shared_ptr<spdlog::logger> console;
        console = spdlog::get("console");

        if (console == nullptr) {
            console = spdlog::stdout_logger_mt("console");
        }

#ifdef FMI4CPP_LOG_LEVEL_OFF
        console->set_level(spdlog::level::off);
#elif FMI4CPP_LOG_LEVEL_INFO
         console->set_level(spdlog::level::info);
#elif FMI4CPP_LOG_LEVEL_DEBUG
        console->set_level(spdlog::level::debug);
#elif FMI4CPP_LOG_LEVEL_TRACE
        console->set_level(spdlog::level::trace);
#elif FMI4CPP_LOG_LEVEL_DEFAULT
        console->set_level(spdlog::level::info);
#endif

        return console;
    };

    const auto console = getOrCreateLogger();

}

namespace fmi4cpp::logger {

    enum class LogLevel {

        LEVEL_OFF,
        LEVEL_INFO,
        LEVEL_WARN,
        LEVEL_DEBUG,
        LEVEL_ERROR
    };

    inline void setLogLevel(const LogLevel level) {
        switch (level) {
            case LogLevel::LEVEL_OFF:
                console->set_level(spdlog::level::off);
                break;
            case LogLevel ::LEVEL_INFO:
                console->set_level(spdlog::level::info);
                break;
            case LogLevel ::LEVEL_WARN:
                console->set_level(spdlog::level::warn);
                break;
            case LogLevel ::LEVEL_DEBUG:
                console->set_level(spdlog::level::debug);
                break;
            case LogLevel ::LEVEL_ERROR:
                console->set_level(spdlog::level::err);
                break;
        }
    }

    template<typename T>
    inline void info(const T &msg) {
        console->info(msg);
    }

    template<typename... Args>
    inline void info(const char* msg, const Args &... args) {
        console->info(msg, args...);
    }

    template<typename T>
    inline void warn(const T &msg) {
        console->warn(msg);
    }

    template<typename... Args>
    inline void warn(const char* msg, const Args &... args) {
        console->warn(msg, args...);
    }

    template<typename T>
    inline void error(const T &msg) {
        console->error(msg);
    }

    template<typename... Args>
    inline void error(const char* msg, const Args &... args) {
        console->error(msg, args...);
    }

    template<typename T>
    inline void debug(const T &msg) {
        console->debug(msg);
    }

    template<typename... Args>
    inline void debug(const char* msg, const Args &... args) {
        console->debug(msg, args...);
    }

    template<typename T>
    inline void trace(const T &msg) {
        console->trace(msg);
    }

    template<typename... Args>
    inline void trace(const char* msg, const Args &... args) {
        console->trace(msg, args...);
    }

}

#endif //FMI4CPP_LOGGER_HPP

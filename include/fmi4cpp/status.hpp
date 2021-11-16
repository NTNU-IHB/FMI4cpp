
#ifndef FMI4CPP_STATUS_HPP
#define FMI4CPP_STATUS_HPP

#include <string>

namespace fmi4cpp
{

enum class status
{
    OK,
    Warning,
    Discard,
    Error,
    Fatal,
    Pending,
    Unknown
};

inline std::string to_string(fmi4cpp::status status)
{
    switch (status) {
        case fmi4cpp::status::OK:
            return "OK";
        case fmi4cpp::status::Warning:
            return "Warning";
        case fmi4cpp::status::Discard:
            return "Discard";
        case fmi4cpp::status::Error:
            return "Error";
        case fmi4cpp::status::Fatal:
            return "Fatal";
        case fmi4cpp::status::Pending:
            return "Pending";
        default:
            return "Unknown";
    }
}

} // namespace fmi4cpp

#endif //FMI4CPP_STATUS_HPP

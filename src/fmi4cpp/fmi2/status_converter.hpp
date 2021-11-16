
#ifndef FMI4CPP_FMI2STATUSCONVERTER_HPP
#define FMI4CPP_FMI2STATUSCONVERTER_HPP

#include <fmi4cpp/fmi2/fmi2FunctionTypes.h>
#include <fmi4cpp/status.hpp>

namespace fmi4cpp::fmi2
{

inline status convert(fmi2Status status)
{
    switch (status) {
        case fmi2OK:
            return status::OK;
        case fmi2Warning:
            return status::Warning;
        case fmi2Discard:
            return status::Discard;
        case fmi2Error:
            return status::Error;
        case fmi2Fatal:
            return status::Fatal;
        case fmi2Pending:
            return status::Pending;
        default:
            return status::Unknown;
    }
}

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2STATUSCONVERTER_HPP

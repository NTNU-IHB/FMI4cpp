//
// Created by LarsIvar on 02.11.2018.
//

#ifndef FMI4CPP_STATUS_CONVERTER_HPP
#define FMI4CPP_STATUS_CONVERTER_HPP

#include <fmi4cpp/Status.hpp>
#include <fmi4cpp/fmi2/fmi2FunctionTypes.h>

namespace fmi4cpp::fmi2 {

    inline Status convert(fmi2Status status) {

        switch (status) {
            case fmi2OK:
                return Status::OK;
            case fmi2Warning:
                return Status::Warning;
            case fmi2Discard:
                return Status::Discard;
            case fmi2Error:
                return Status::Error;
            case fmi2Fatal:
                return Status::Fatal;
            case fmi2Pending:
                return Status::Pending;
        }

    }

}

#endif //FMI4CPP_STATUS_CONVERTER_HPP

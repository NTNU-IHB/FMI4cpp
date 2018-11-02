//
// Created by LarsIvar on 02.11.2018.
//

#ifndef FMI4CPP_STATUS_HPP
#define FMI4CPP_STATUS_HPP

namespace fmi4cpp {

    enum class Status {
        OK,
        Warning,
        Discard,
        Error,
        Fatal,
        Pending
    };

}

#endif //FMI4CPP_STATUS_HPP

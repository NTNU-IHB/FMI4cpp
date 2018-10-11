//
// Created by laht on 10.10.18.
//

#include "optional_converter.hpp"

template<class T>
std::optional<T> convert(boost::optional<T> opt) {
    if (!opt) {
        return {};
    } else {
        return *opt;
    }
}
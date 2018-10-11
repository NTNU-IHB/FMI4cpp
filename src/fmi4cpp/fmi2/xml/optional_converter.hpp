//
// Created by laht on 10.10.18.
//

#ifndef FMI4CPP_OPTIONAL_CONVERTER_HPP
#define FMI4CPP_OPTIONAL_CONVERTER_HPP

#include <optional>
#include <boost/optional/optional.hpp>

template<class T>
std::optional<T> convert(boost::optional<T> opt);

#endif //FMI4CPP_OPTIONAL_CONVERTER_HPP

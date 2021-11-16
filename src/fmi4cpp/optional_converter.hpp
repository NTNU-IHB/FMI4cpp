
#ifndef FMI4CPP_OPTIONAL_CONVERTER_HPP
#define FMI4CPP_OPTIONAL_CONVERTER_HPP

#include <boost/optional.hpp>

#include <optional>

namespace
{

template<class T>
std::optional<T> convert(boost::optional<T> opt)
{
    if (!opt) {
        return std::nullopt;
    } else {
        return *opt;
    }
}

} // namespace

#endif //FMI4CPP_OPTIONAL_CONVERTER_HPP

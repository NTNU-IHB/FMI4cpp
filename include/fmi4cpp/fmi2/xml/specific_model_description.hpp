
#ifndef FMI4CPP_SPECIFICMODELDESCRIPTION_HPP
#define FMI4CPP_SPECIFICMODELDESCRIPTION_HPP

#include <fmi4cpp/fmi2/xml/model_description.hpp>

namespace fmi4cpp::fmi2
{

template<typename T>
struct specific_model_description : model_description_base, T
{
    specific_model_description(const model_description_base& base, const T& attributes)
        : model_description_base(base)
        , T(attributes)
    {}
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_SPECIFICMODELDESCRIPTION_HPP

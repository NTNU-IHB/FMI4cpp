
#ifndef FMI4CPP_FMI2_CS_MODEL_DESCRIPTION_HPP
#define FMI4CPP_FMI2_CS_MODEL_DESCRIPTION_HPP

#include <fmi4cpp/fmi2/xml/specific_model_description.hpp>

namespace fmi4cpp::fmi2
{

struct cs_model_description : specific_model_description<cs_attributes>
{
    cs_model_description(const model_description_base& base, const cs_attributes& attributes)
        : specific_model_description(base, attributes)
    {}
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2_CS_MODEL_DESCRIPTION_HPP

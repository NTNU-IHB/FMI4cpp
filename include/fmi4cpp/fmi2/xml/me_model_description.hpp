
#ifndef FMI4CPP_FMI2_ME_MODEL_DESCRIPTION_HPP
#define FMI4CPP_FMI2_ME_MODEL_DESCRIPTION_HPP

#include <fmi4cpp/fmi2/xml/specific_model_description.hpp>

namespace fmi4cpp::fmi2
{

struct me_model_description : public specific_model_description<me_attributes>
{
    me_model_description(const model_description_base& base, const me_attributes& attributes)
        : specific_model_description(base, attributes)
    {}
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2_ME_MODEL_DESCRIPTION_HPP

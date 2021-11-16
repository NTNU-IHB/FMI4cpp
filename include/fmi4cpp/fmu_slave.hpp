
#ifndef FMI4CPP_FMUSLAVE_HPP
#define FMI4CPP_FMUSLAVE_HPP

#include <fmi4cpp/fmu_instance.hpp>

namespace fmi4cpp
{

template<typename cs_model_description>
class fmu_slave : public virtual fmu_instance<cs_model_description>
{

public:
    virtual bool step(double stepSize) = 0;

    virtual bool cancel_step() = 0;
};

} // namespace fmi4cpp

#endif //FMI4CPP_FMUSLAVE_HPP

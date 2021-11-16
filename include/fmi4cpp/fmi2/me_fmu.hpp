
#ifndef FMI4CPP_FMI2MODELEXCHANGEFMU_H
#define FMI4CPP_FMI2MODELEXCHANGEFMU_H

#include <fmi4cpp/fmi2/me_instance.hpp>
#include <fmi4cpp/fmi2/xml/me_model_description.hpp>
#include <fmi4cpp/fmu_base.hpp>
#include <fmi4cpp/fmu_resource.hpp>

namespace fmi4cpp::fmi2
{

class me_fmu : public virtual me_fmu_base<me_instance, me_model_description>
{

private:
    std::shared_ptr<fmu_resource> resource_;
    std::shared_ptr<me_library> lib_;
    std::shared_ptr<const me_model_description> modelDescription_;

public:
    me_fmu(std::shared_ptr<fmu_resource> resource,
        std::shared_ptr<const me_model_description> md);

    [[nodiscard]] std::string get_model_description_xml() const;

    [[nodiscard]] std::shared_ptr<const me_model_description> get_model_description() const override;

    std::unique_ptr<me_instance> new_instance(bool visible = false, bool loggingOn = false);
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2MODELEXCHANGEFMU_H


#ifndef FMI4CPP_FMI2COSIMULATIONFMU_H
#define FMI4CPP_FMI2COSIMULATIONFMU_H

#include <fmi4cpp/fmi2/cs_library.hpp>
#include <fmi4cpp/fmi2/cs_slave.hpp>
#include <fmi4cpp/fmi2/xml/cs_model_description.hpp>
#include <fmi4cpp/fmu_base.hpp>
#include <fmi4cpp/fmu_resource.hpp>


namespace fmi4cpp::fmi2
{

class cs_fmu : public cs_fmu_base<cs_slave, cs_model_description>
{

private:
    std::shared_ptr<cs_library> lib_;
    std::shared_ptr<fmu_resource> resource_;
    std::shared_ptr<const cs_model_description> modelDescription_;

public:
    cs_fmu(std::shared_ptr<fmu_resource> resource,
        std::shared_ptr<const cs_model_description> md);

    [[nodiscard]] std::string get_model_description_xml() const;

    [[nodiscard]] std::shared_ptr<const cs_model_description> get_model_description() const override;

    std::unique_ptr<cs_slave> new_instance(bool visible = false, bool loggingOn = false) override;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2COSIMULATIONFMU_H

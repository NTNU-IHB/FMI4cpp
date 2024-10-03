
#ifndef FMI4CPP_FMI2FMU_HPP
#define FMI4CPP_FMI2FMU_HPP

#include <fmi4cpp/fmi2/cs_fmu.hpp>
#include <fmi4cpp/fmi2/me_fmu.hpp>
#include <fmi4cpp/fmi2/xml/me_model_description.hpp>
#include <fmi4cpp/fmu_base.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace fmi4cpp::fmi2
{

class fmu : public virtual fmu_provider<model_description, cs_fmu, me_fmu>
{

    friend class cs_fmu;
    friend class me_fmu;

private:
    std::shared_ptr<fmu_resource> resource_;
    std::shared_ptr<const model_description> modelDescription_;

public:
    explicit fmu(const std::filesystem::path& fmuPath);

    [[nodiscard]] std::string get_model_description_xml() const;
    [[nodiscard]] std::shared_ptr<const model_description> get_model_description() const override;

    [[nodiscard]] bool supports_me() const override;
    [[nodiscard]] bool supports_cs() const override;

    [[nodiscard]] std::unique_ptr<cs_fmu> as_cs_fmu() const override;
    [[nodiscard]] std::unique_ptr<me_fmu> as_me_fmu() const override;
};

} // namespace fmi4cpp::fmi2


#endif // FMI4CPP_FMI2FMU_HPP

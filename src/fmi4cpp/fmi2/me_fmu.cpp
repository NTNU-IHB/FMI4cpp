
#include <fmi4cpp/fmi2/me_fmu.hpp>
#include <fmi4cpp/fmi2/me_instance.hpp>

#include <utility>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

me_fmu::me_fmu(std::shared_ptr<fmu_resource> resource,
    std::shared_ptr<const me_model_description> md)
    : resource_(std::move(resource))
    , modelDescription_(std::move(md))
{}

std::string me_fmu::get_model_description_xml() const
{
    return resource_->get_model_description_xml();
}

std::shared_ptr<const me_model_description> me_fmu::get_model_description() const
{
    return modelDescription_;
}

std::unique_ptr<me_instance> me_fmu::new_instance(const bool visible, const bool loggingOn)
{
    std::shared_ptr<me_library> lib = nullptr;
    auto modelIdentifier = modelDescription_->model_identifier;
    if (lib_ == nullptr) {
        lib_ = std::make_shared<me_library>(modelIdentifier, resource_);
    }
    lib = lib_;

    fmi2Component c = lib->instantiate(modelIdentifier, fmi2ModelExchange, guid(),
        resource_->resource_path(), visible, loggingOn);
    return std::make_unique<me_instance>(c, resource_, lib, modelDescription_);
}

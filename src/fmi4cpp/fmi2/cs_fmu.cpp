
#include <fmi4cpp/fmi2/cs_fmu.hpp>

#include <utility>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

cs_fmu::cs_fmu(std::shared_ptr<fmu_resource> resource,
               std::shared_ptr<const cs_model_description> md)
    : resource_(std::move(resource))
    , modelDescription_(std::move(md))
{}

std::string cs_fmu::get_model_description_xml() const
{
    return resource_->get_model_description_xml();
}

std::shared_ptr<const cs_model_description> cs_fmu::get_model_description() const
{
    return modelDescription_;
}

std::unique_ptr<cs_slave> cs_fmu::new_instance(const bool visible, const bool loggingOn)
{
    std::shared_ptr<cs_library> lib = nullptr;
    auto modelIdentifier = modelDescription_->model_identifier;
    if (lib_ == nullptr) {
        lib_ = std::make_shared<cs_library>(modelIdentifier, resource_);
    }
    lib = lib_;

    auto c = lib->instantiate(modelIdentifier, fmi2CoSimulation, guid(),
        resource_->resource_path(), visible, loggingOn);
    return std::make_unique<cs_slave>(c, resource_, lib, modelDescription_);
}

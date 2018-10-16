//
// Created by LarsIvar on 14.10.2018.
//

#include <fmi4cpp/fmi2/import/Fmu.hpp>
#include <fmi4cpp/fmi2/import/ModelExchangeInstance.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

import::ModelExchangeFmu::ModelExchangeFmu(const shared_ptr<import::FmuResource> &resource,
                                           const shared_ptr<xml::ModelExchangeModelDescription> &md)
        : resource_(resource), modelDescription_(md) {}


shared_ptr<xml::ModelExchangeModelDescription> import::ModelExchangeFmu::getModelDescription() const {
    return modelDescription_;
}

std::unique_ptr<import::ModelExchangeInstance> import::ModelExchangeFmu::newInstance(const bool visible, const bool loggingOn) {
    shared_ptr<ModelExchangeLibrary> lib = nullptr;
    string modelIdentifier = modelDescription_->modelIdentifier();
    if (modelDescription_->canBeInstantiatedOnlyOncePerProcess()) {
        lib = make_shared<ModelExchangeLibrary>(modelIdentifier, resource_);
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<ModelExchangeLibrary>(modelIdentifier, resource_);
        }
        lib = lib_;
    }
    fmi2Component c = lib->instantiate(modelIdentifier, fmi2ModelExchange, guid(),
                                       resource_->getResourcePath(), visible, loggingOn);
    return make_unique<import::ModelExchangeInstance>(c, lib, modelDescription_);
}

import::ModelExchangeFmu::~ModelExchangeFmu() {
//#if  FMI4CPP_DEBUG_LOGGING_ENABLED
//    cout << "~ModelExchangeFmu()" << endl;
//#endif
}
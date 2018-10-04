/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if FMI4CPP_DEBUG_LOGGING_ENABLED
#include <iostream>
#endif

#include <random>
#include <fmi4cpp/fmi2/import/Fmu.hpp>
#include <fmi4cpp/fmi2/import/CoSimulationSlave.hpp>
#include <fmi4cpp/fmi2/import/ModelExchangeInstance.hpp>

#include "../../tools/unzipper.hpp"
#include "../../tools/os_util.hpp"

using namespace std;
using namespace fmi4cpp::fmi2;

using fmi4cpp::fmi2::import::Fmu;
using fmi4cpp::fmi2::import::CoSimulationFmu;
using fmi4cpp::fmi2::import::ModelExchangeFmu;

namespace {

    std::random_device rd;
    std::mt19937 mt(rd());

    string generate_simple_id() {

        std::uniform_int_distribution<int> dist(0, 10);
        string id;
        for (int i = 0; i < 8; i++) {
            id += std::to_string(dist(mt));
        }
        return id;
    }

}

Fmu::Fmu(const string &fmuFile) : fmuFile_(fmuFile) {

    const string fmuName = fs::path(fmuFile).stem().string();
    tmpPath_ = fs::temp_directory_path() /= fs::path("fmi4cpp_" + fmuName + "_" + generate_simple_id());

    if (!create_directories(tmpPath_)) {
        throw runtime_error("Failed to create temporary directory!");
    }

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "Created temporary directory '" << tmpPath_.string()  << "'" << endl;
#endif
    if (!extractContents(fmuFile, tmpPath_.string())) {
        throw runtime_error("Failed to extract FMU!");
    }

    modelDescription_ = make_unique<xml::ModelDescriptionImpl>(getModelDescriptionPath());

}

string Fmu::getModelDescriptionXml() const {
    ifstream stream(getModelDescriptionPath());
    const string xml = string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
    return xml;
}

const ModelDescriptionProvider &Fmu::getModelDescription() const {
    return *modelDescription_;
}

bool Fmu::supportsModelExchange() const {
    return modelDescription_->supportsModelExchange();
}

 bool Fmu::supportsCoSimulation() const {
    return modelDescription_->supportsCoSimulation();
}

CoSimulationFmu &Fmu::asCoSimulationFmu() {
    if (!csFmu) {
        csFmu = make_unique<CoSimulationFmu>(*this);
    }
    return *csFmu;
}

ModelExchangeFmu &Fmu::asModelExchangeFmu() {
    if (!meFmu) {
        meFmu = make_unique<ModelExchangeFmu>(*this);
    }
    return *meFmu;
}

string Fmu::getAbsoluteLibraryPath(const string modelIdentifier) const {
    return tmpPath_.string() + "/binaries/" + getOs() + "/" + modelIdentifier + getLibExt();
}

string Fmu::getResourcePath() const {
    return "file:/" + tmpPath_.string() + "/resources/" + getOs() + "/" + getLibExt();
}

string Fmu::getModelDescriptionPath() const {
    return tmpPath_.string() + "/modelDescription.xml";
}

Fmu::~Fmu() {

    remove_all(tmpPath_);

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "FMU '" << getModelName() << "' disposed.." << endl;
#endif

}


const xml::CoSimulationModelDescription &CoSimulationFmu::getModelDescription() const {
    return modelDescription_;
}

unique_ptr<import::FmuSlave> CoSimulationFmu::newInstance(const bool visible, const bool loggingOn) {
    shared_ptr<import::CoSimulationLibrary> lib = nullptr;
    string modelIdentifier = modelDescription_.getModelIdentifier();
    if (modelDescription_.canBeInstantiatedOnlyOncePerProcess()) {
        lib = make_shared<CoSimulationLibrary>(fmu_.getAbsoluteLibraryPath(modelIdentifier));
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<CoSimulationLibrary>(fmu_.getAbsoluteLibraryPath(modelIdentifier));
        }
        lib = lib_;
    }
    fmi2Component c = lib->instantiate(modelIdentifier, fmi2CoSimulation, getGuid(),
                                       fmu_.getResourcePath(), visible, loggingOn);
    return make_unique<import::CoSimulationSlave>(c, lib, modelDescription_);
}


const xml::ModelExchangeModelDescription &ModelExchangeFmu::getModelDescription() const {
    return modelDescription_;
}

std::unique_ptr<import::ModelExchangeInstance> ModelExchangeFmu::newInstance(const bool visible, const bool loggingOn) {
    shared_ptr<ModelExchangeLibrary> lib = nullptr;
    string modelIdentifier = modelDescription_.getModelIdentifier();
    if (modelDescription_.canBeInstantiatedOnlyOncePerProcess()) {
        lib = make_shared<ModelExchangeLibrary>(fmu_.getAbsoluteLibraryPath(modelIdentifier));
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<ModelExchangeLibrary>(fmu_.getAbsoluteLibraryPath(modelIdentifier));
        }
        lib = lib_;
    }
    fmi2Component c = lib->instantiate(modelIdentifier, fmi2ModelExchange, getGuid(),
                                       fmu_.getResourcePath(), visible, loggingOn);
    return make_unique<ModelExchangeInstance>(c, lib, modelDescription_);
}

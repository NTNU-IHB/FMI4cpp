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
#include <experimental/filesystem>

#include <fmi4cpp/fmi2/import/Fmu.hpp>
#include <fmi4cpp/fmi2/import/CoSimulationSlave.hpp>
#include <fmi4cpp/fmi2/import/ModelExchangeInstance.hpp>

#include "../xml/ModelDescriptionParser.hpp"

#include "../../tools/unzipper.hpp"
#include "../../tools/os_util.hpp"

using namespace std;
using namespace fmi4cpp::fmi2;

using fmi4cpp::fmi2::import::Fmu;
using fmi4cpp::fmi2::import::CoSimulationFmu;
using fmi4cpp::fmi2::import::ModelExchangeFmu;

namespace fs = std::experimental::filesystem;


namespace {

    std::random_device rd;
    std::mt19937 mt(rd());

    const std::string generate_simple_id() {

        std::string id;
        std::uniform_int_distribution<int> dist(0, 9);
        for (int i = 0; i < 8; i++) {
            id += std::to_string(dist(mt));
        }
        return id;
    }

    const std::string getAbsoluteLibraryPath(const fs::path &path, const std::string &modelIdentifier) {
        return path.string() + "/binaries/" + getOs() + "/" + modelIdentifier + getLibExt();
    }

    const std::string getResourcePath(const fs::path &path) {
        return "file:/" + path.string() + "/resources/" + getOs() + "/" + getLibExt();
    }

}

Fmu::Fmu(const string &fmuFile) : fmuFile_(fmuFile) {

    const string fmuName = fs::path(fmuFile).stem().string();
    fs::path tmpPath(fs::temp_directory_path() /= fs::path("fmi4cpp_" + fmuName + "_" + generate_simple_id()));

    if (!create_directories(tmpPath)) {
        throw runtime_error("Failed to create temporary directory!");
    }

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "Created temporary directory '" << tmpPath.string() << "'" << endl;
#endif

    if (!extractContents(fmuFile, tmpPath.string())) {
        throw runtime_error("Failed to extract FMU!");
    }

    tmpFolder_ = make_shared<TemporalFolder>(tmpPath);
    modelDescription_ = std::move(xml::parseModelDescription(getModelDescriptionPath()));

}

const string Fmu::getModelDescriptionXml() const {
    ifstream stream(getModelDescriptionPath());
    return string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
}

shared_ptr<ModelDescription> Fmu::getModelDescription() const {
    return modelDescription_;
}

bool Fmu::supportsModelExchange() const {
    return modelDescription_->supportsModelExchange();
}

bool Fmu::supportsCoSimulation() const {
    return modelDescription_->supportsCoSimulation();
}

unique_ptr<CoSimulationFmu> Fmu::asCoSimulationFmu() const {
    shared_ptr<CoSimulationModelDescription> cs = std::move(modelDescription_->asCoSimulationModelDescription());
    return make_unique<CoSimulationFmu>(tmpFolder_, cs);
}

unique_ptr<ModelExchangeFmu> Fmu::asModelExchangeFmu() const {
    shared_ptr<ModelExchangeModelDescription> me = std::move(modelDescription_->asModelExchangeModelDescription());
    return make_unique<ModelExchangeFmu>(tmpFolder_, me);
}

const string Fmu::getModelDescriptionPath() const {
    return tmpFolder_->string() + "/modelDescription.xml";
}

Fmu::~Fmu() {

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "FMU '" << modelName() << "' disposed.." << endl;
#endif

}

import::CoSimulationFmu::CoSimulationFmu(const shared_ptr<import::TemporalFolder> &tmpFolder,
                                         const shared_ptr<xml::CoSimulationModelDescription> &md)
        : tmpFolder_(tmpFolder), modelDescription_(md) {}

shared_ptr<xml::CoSimulationModelDescription> CoSimulationFmu::getModelDescription() const {
    return modelDescription_;
}

unique_ptr<import::FmuSlave> CoSimulationFmu::newInstance(const bool visible, const bool loggingOn) {
    shared_ptr<import::CoSimulationLibrary> lib = nullptr;
    string modelIdentifier = modelDescription_->modelIdentifier();
    if (modelDescription_->canBeInstantiatedOnlyOncePerProcess()) {
        lib = make_shared<CoSimulationLibrary>(getAbsoluteLibraryPath(*tmpFolder_, modelIdentifier));
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<CoSimulationLibrary>(getAbsoluteLibraryPath(*tmpFolder_, modelIdentifier));
        }
        lib = lib_;
    }
    fmi2Component c = lib->instantiate(modelIdentifier, fmi2CoSimulation, guid(),
                                       getResourcePath(*tmpFolder_), visible, loggingOn);
    return make_unique<CoSimulationSlave>(c, lib, modelDescription_);
}

import::ModelExchangeFmu::ModelExchangeFmu(const shared_ptr<import::TemporalFolder> &tmpFolder,
                                           const shared_ptr<xml::ModelExchangeModelDescription> &md)
        : tmpFolder_(tmpFolder), modelDescription_(md) {}


shared_ptr<xml::ModelExchangeModelDescription> ModelExchangeFmu::getModelDescription() const {
    return modelDescription_;
}

std::unique_ptr<import::ModelExchangeInstance> ModelExchangeFmu::newInstance(const bool visible, const bool loggingOn) {
    shared_ptr<ModelExchangeLibrary> lib = nullptr;
    string modelIdentifier = modelDescription_->modelIdentifier();
    if (modelDescription_->canBeInstantiatedOnlyOncePerProcess()) {
        lib = make_shared<ModelExchangeLibrary>(getAbsoluteLibraryPath(*tmpFolder_, modelIdentifier));
    } else {
        if (lib_ == nullptr) {
            lib_ = make_shared<ModelExchangeLibrary>(getAbsoluteLibraryPath(*tmpFolder_, modelIdentifier));
        }
        lib = lib_;
    }
    fmi2Component c = lib->instantiate(modelIdentifier, fmi2ModelExchange, guid(),
                                       getResourcePath(*tmpFolder_), visible, loggingOn);
    return make_unique<ModelExchangeInstance>(c, lib, modelDescription_);
}


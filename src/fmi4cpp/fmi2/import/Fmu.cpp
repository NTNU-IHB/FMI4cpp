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

#include <fmi4cpp/fmi2/import/Fmu.hpp>
#include <fmi4cpp/fmi2/import/CoSimulationSlaveBuilder.hpp>
#include <fmi4cpp/fmi2/import/ModelExchangeInstanceBuilder.hpp>

#include <fmi4cpp/tools/unzipper.hpp>
#include <fmi4cpp/tools/os_util.hpp>

using namespace std;
using namespace fmi4cpp::fmi2::import;

namespace {

    string generate_simple_id() {
        string id;
        for (int i = 0; i < 8; i++) {
            id += to_string(rand() % 10);
        }
        return id;
    }

}

Fmu::Fmu(const string &fmu_file) : fmu_file_(fmu_file) {

    const string fmuName = fs::path(fmu_file).stem().string();
    tmp_path_ = fs::temp_directory_path() /= fs::path(fmuName + "_" + generate_simple_id());

    if (!create_directories(tmp_path_)) {
        throw runtime_error("Failed to create temporary directory!");
    }

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "Created temporary directory '" << tmp_path_.string()  << "'" << endl;
#endif
    if (!extractContents(fmu_file, tmp_path_.string())) {
        throw runtime_error("Failed to extract FMU!");
    }

    modelDescription_ = make_unique<ModelDescription>();
    modelDescription_->load(getModelDescriptionPath());

}

const string Fmu::getGuid() const {
    return modelDescription_->guid();
}

const string Fmu::getModelName() const {
    return modelDescription_->modelName();
}

const string Fmu::getModelDescriptionXml() const {
    ifstream stream(getModelDescriptionPath());
    const string xml = string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
    return xml;
}

const ModelDescription &Fmu::getModelDescription() const {
    return *modelDescription_;
}

const bool Fmu::supportsModelExchange() const {
    return modelDescription_->supportsModelExchange();
}

const bool Fmu::supportsCoSimulation() const {
    return modelDescription_->supportsCoSimulation();
}

CoSimulationSlaveBuilder &Fmu::asCoSimulationFmu() {
    if (csBuilder_ == nullptr) {
        csBuilder_ = make_unique<CoSimulationSlaveBuilder>(*this);
    }
    return *csBuilder_;
}

ModelExchangeInstanceBuilder &Fmu::asModelExchangeFmu() {
    if (meBuilder_ == nullptr) {
        meBuilder_ = make_unique<ModelExchangeInstanceBuilder>(*this);
    }
    return *meBuilder_;
}

const string Fmu::getAbsoluteLibraryPath(const string modelIdentifier) const {
    return tmp_path_.string() + "/binaries/" + getOs() + "/" + modelIdentifier + getLibExt();
}

const string Fmu::getResourcePath() const {
    return "file:/" + tmp_path_.string() + "/resources/" + getOs() + "/" + getLibExt();
}

const string Fmu::getModelDescriptionPath() const {
    return tmp_path_.string() + "/modelDescription.xml";
}

Fmu::~Fmu() {
    meBuilder_ = nullptr;
    csBuilder_ = nullptr;
    remove_all(tmp_path_);

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "Fmu '" << getModelName() << "' disposed.." << endl;
#endif

}

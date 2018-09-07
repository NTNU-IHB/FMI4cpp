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


#include <fmicpp/fmi2/import/Fmu.hpp>
#include <fmicpp/fmi2/import/CoSimulationSlaveBuilder.hpp>
#include <fmicpp/fmi2/import/ModelExchangeInstanceBuilder.hpp>

#include <fmicpp/tools/unzipper.hpp>
#include <fmicpp/tools/os_util.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>


using namespace std;
using namespace fmicpp::fmi2::import;

namespace {

    const string generate_uuid() {
        using namespace boost::uuids;
        random_generator generator;
        uuid uuid = generator();
        return to_string(uuid);
    }

}

Fmu::Fmu(const string fmu_file): fmu_file_(fmu_file) {

    const string fmuName = fs::path(fmu_file).stem().string();
    tmp_path_ = fs::temp_directory_path() /= fs::path(fmuName + "_" + generate_uuid());
    create_directories(tmp_path_);

    if (!extractContents(fmu_file, tmp_path_.string())) {
        throw runtime_error("Failed to extract FMU!");
    }

    const string modelDescriptionPath = tmp_path_.string() + "/modelDescription.xml";

    modelDescription_ = make_unique<ModelDescription>();
    modelDescription_->load(modelDescriptionPath);

    ifstream t(modelDescriptionPath);
    model_description_xml_ = string((istreambuf_iterator<char>(t)),
                                          istreambuf_iterator<char>());

}

const string Fmu::getGuid() const {
    return modelDescription_->guid;
}

const string Fmu::getModelName() const {
    return modelDescription_->modelName;
}

const string &Fmu::getModelDescriptionXml() const {
    return model_description_xml_;
}

const ModelDescription &Fmu::getModelDescription() const {
    return *modelDescription_;
}

const bool Fmu::supportsModelExchange() const {
    return modelDescription_->supportsModelExchange;
}

const bool Fmu::supportsCoSimulation() const {
    return modelDescription_->supportsCoSimulation;
}

CoSimulationSlaveBuilder Fmu::asCoSimulationFmu() {
    return CoSimulationSlaveBuilder(*this);
}

ModelExchangeInstanceBuilder Fmu::asModelExchangeFmu() {
    return ModelExchangeInstanceBuilder(*this);
}

const string Fmu::getAbsoluteLibraryPath(string modelIdentifier) const {
    return tmp_path_.string() + "/binaries/" + getOs() + "/" + modelIdentifier + getLibExt();
}

const string Fmu::getResourcePath() const {
    return "file:/" + tmp_path_.string() + "/resources/" + getOs() + "/" + getLibExt();
}

Fmu::~Fmu() {
    remove_all(tmp_path_);
}

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

}

import::Fmu::Fmu(const string &fmuFile) {

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

    resource_ = make_shared<FmuResource>(tmpPath);
    modelDescription_ = std::move(xml::parseModelDescription(resource_->getModelDescriptionPath()));

}


const string import::Fmu::getModelDescriptionXml() const {
    ifstream stream(resource_->getModelDescriptionPath());
    return string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
}

shared_ptr<ModelDescription> import::Fmu::getModelDescription() const {
    return modelDescription_;
}

bool import::Fmu::supportsModelExchange() const {
    return modelDescription_->supportsModelExchange();
}

bool import::Fmu::supportsCoSimulation() const {
    return modelDescription_->supportsCoSimulation();
}

import::Fmu::~Fmu() {
//#if  FMI4CPP_DEBUG_LOGGING_ENABLED
//    cout << "~Fmu()" << endl;
//#endif
}




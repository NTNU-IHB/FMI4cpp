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

#include <experimental/filesystem>

#include <fmi4cpp/fmi2/import/Fmu.hpp>
#include <fmi4cpp/fmi2/import/CoSimulationSlave.hpp>
#include <fmi4cpp/fmi2/import/ModelExchangeInstance.hpp>

#include "../xml/ModelDescriptionParser.hpp"

#include "../../tools/unzipper.hpp"
#include "../../tools/os_util.hpp"
#include "../../tools/simple_id.hpp"

using namespace std;
using namespace fmi4cpp::fmi2;

namespace fs = std::experimental::filesystem;

Fmu::Fmu(const string &fmuFile): fmuFile_(fmuFile) {

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "Loading FMU '" << fmuFile << "'" << endl;
#endif

    const string fmuName = fs::path(fmuFile).stem().string();
    fs::path tmpPath(fs::temp_directory_path() /= fs::path("fmi4cpp_" + fmuName + "_" + generate_simple_id(8)));

    if (!create_directories(tmpPath)) {
        string err = "Failed to create temporary directory!";
#if FMI4CPP_DEBUG_LOGGING_ENABLED
        cerr << err << endl;
#endif
        throw runtime_error(err);
    }

#if FMI4CPP_DEBUG_LOGGING_ENABLED
    cout << "Created temporary directory '" << tmpPath.string() << "'" << endl;
#endif

    if (!extractContents(fmuFile, tmpPath.string())) {
        const string err = "Failed to extract FMU!";
#if FMI4CPP_DEBUG_LOGGING_ENABLED
        cout << err << endl;
#endif
        throw runtime_error(err);
    }

    resource_ = make_shared<FmuResource>(tmpPath);
    modelDescription_ = std::move(parseModelDescription(resource_->getModelDescriptionPath()));

}


const string Fmu::getModelDescriptionXml() const {
    ifstream stream(resource_->getModelDescriptionPath());
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

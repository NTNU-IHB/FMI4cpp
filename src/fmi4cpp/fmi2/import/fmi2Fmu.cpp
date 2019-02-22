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

#ifdef FMI4CPP_WITH_CURL
#include <curl/curl.h>
#endif

#include <utility>
#include <experimental/filesystem>

#include <fmi4cpp/fmi2/import/fmi2Fmu.hpp>
#include <fmi4cpp/fmi2/xml/ModelDescriptionParser.hpp>

#include "../../common//logger.hpp"
#include "../../common/tools/unzipper.hpp"
#include "../../common/tools/os_util.hpp"
#include "../../common/tools/simple_id.hpp"

using namespace std;
using namespace fmi4cpp::fmi2;

namespace fs = std::experimental::filesystem;

fmi2Fmu::fmi2Fmu(string_view fmuPath): fmuName_(fs::path(fmuPath.data()).stem().string()) {

    fmi4cpp::logger::debug("Loading FMU '{}'", fmuPath.data());

    fs::path tmpPath(fs::temp_directory_path() /= fs::path("fmi4cpp_" + fmuName_ + "_" + generate_simple_id(8)));

    if (!create_directories(tmpPath)) {
        auto err = "Failed to create temporary directory '" + tmpPath.string() + "' !";
        fmi4cpp::logger::error(err);
        throw runtime_error(err);
    }

    fmi4cpp::logger::debug("Created temporary directory '{}'", tmpPath.string());

    if (!extractContents(fmuPath, tmpPath.string())) {
        auto err = "Failed to extract FMU '" + string(fmuPath) + "'!";
        fmi4cpp::logger::error(err);
        throw runtime_error(err);
    }

    resource_ = make_shared<FmuResource>(tmpPath);
    modelDescription_ = std::move(parseModelDescription(resource_->getModelDescriptionPath()));

}
 std::string_view fmi2Fmu::getFmuName() const {
    return fmuName_;
}

 string_view fmi2Fmu::getModelDescriptionXml() const {
    ifstream stream(resource_->getModelDescriptionPath());
    return string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
}

shared_ptr<const ModelDescription> fmi2Fmu::getModelDescription() const {
    return modelDescription_;
}

bool fmi2Fmu::supportsModelExchange() const {
    return modelDescription_->supportsModelExchange();
}

bool fmi2Fmu::supportsCoSimulation() const {
    return modelDescription_->supportsCoSimulation();
}

unique_ptr<fmi2CoSimulationFmu> fmi2Fmu::asCoSimulationFmu() const {
    shared_ptr<const CoSimulationModelDescription> cs = std::move(modelDescription_->asCoSimulationModelDescription());
    return make_unique<fmi2CoSimulationFmu>(resource_, cs);
}

unique_ptr<fmi2ModelExchangeFmu> fmi2Fmu::asModelExchangeFmu() const {
    shared_ptr<const ModelExchangeModelDescription> me = std::move(modelDescription_->asModelExchangeModelDescription());
    return make_unique<fmi2ModelExchangeFmu>(resource_, me);
}

#ifdef FMI4CPP_WITH_CURL

namespace {
    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
    }
}

std::unique_ptr<fmi2Fmu> fmi2Fmu::fromUrl(std::string_view fmuPath) {

    fmi4cpp::logger::debug("Loading FMU from URL: {}", fmuPath.data());

    auto fmuName = fs::path(fmuPath).filename();
    fs::path tmp(fs::temp_directory_path() /= fmuName);
    CURL *curl = curl_easy_init();
    if (curl) {
        FILE *fp = fopen(tmp.string().c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, fmuPath.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            auto err = "Failed to download FMU from URL:" + string(fmuPath);
            fmi4cpp::logger::error(err);
            throw runtime_error(err);
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    auto fmu = std::make_unique<fmi2Fmu>(tmp.string());
    fs::remove(tmp); //delete downloaded FMU, it has been extracted by now.
    return fmu;

}
#endif
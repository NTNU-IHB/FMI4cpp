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

#include <fmi4cpp/fmi2/fmu.hpp>
#include <fmi4cpp/fmi2/xml/model_description_parser.hpp>

#include <fmi4cpp/logger.hpp>
#include <fmi4cpp/tools/unzipper.hpp>
#include <fmi4cpp/tools/os_util.hpp>
#include <fmi4cpp/tools/simple_id.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

namespace fs = std::experimental::filesystem;

fmu::fmu(const string &fmuPath) {

    FMI4CPP_DEBUG("Loading FMU '" <<  fmuPath << "'");

    const std::string fmuName = fs::path(fmuPath).stem().string();
    fs::path tmpPath(fs::temp_directory_path() /= fs::path("fmi4cpp_" + fmuName + "_" + generate_simple_id(8)));

    if (!create_directories(tmpPath)) {
        auto err = "Failed to create temporary directory '" + tmpPath.string() + "' !";
        FMI4CPP_ERROR(err);
        throw runtime_error(err);
    }

    FMI4CPP_DEBUG("Created temporary directory '" << tmpPath.string());

    if (!unzip(fmuPath, tmpPath.string())) {
        auto err = "Failed to extract FMU '" + fmuPath + "'!";
        FMI4CPP_ERROR(err);
        throw runtime_error(err);
    }

    resource_ = make_shared<fmu_resource>(tmpPath);
    modelDescription_ = std::move(parse_model_description(resource_->model_description_path()));

}

const std:: string fmu::model_description_xml() const {
    ifstream stream(resource_->model_description_path());
    return string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
}

shared_ptr<const model_description> fmu::model_description() const {
    return modelDescription_;
}

bool fmu::supports_me() const {
    return modelDescription_->supports_me();
}

bool fmu::supports_cs() const {
    return modelDescription_->supports_cs();
}

unique_ptr<cs_fmu> fmu::as_cs_fmu() const {
    shared_ptr<const cs_model_description> cs = std::move(modelDescription_->as_cs_model_description());
    return make_unique<cs_fmu>(resource_, cs);
}

unique_ptr<me_fmu> fmu::as_me_fmu() const {
    shared_ptr<const me_model_description> me = std::move(modelDescription_->as_me_description());
    return make_unique<me_fmu>(resource_, me);
}



namespace {
    size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
    }
}

std::unique_ptr<fmu> fmu::from_url(const std::string &fmuPath) {

#ifdef FMI4CPP_WITH_CURL

    FMI4CPP_DEBUG("Loading FMU from URL: " << fmuPath);

    auto fmuName = fs::path(fmuPath).filename();
    fs::path tmp(fs::temp_directory_path() /= fmuName);
    CURL *curl = curl_easy_init();
    if (curl) {
        FILE *fp = fopen(tmp.string().c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, fmuPath.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            auto err = "Failed to download FMU from URL:" + fmuPath;
            FMI4CPP_ERROR(err);
            throw runtime_error(err);
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    auto fmu = std::make_unique<fmi2::fmu>(tmp.string());
    fs::remove(tmp); //delete downloaded FMU, it has been extracted by now.
    return fmu;

#else

    throw runtime_error("fmi2Fmu::fromUrl not enabled! To enable compile with FMI4CPP_WITH_CURL=ON");

#endif

}
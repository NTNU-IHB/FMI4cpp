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


#include <fmi4cpp/logger.hpp>
#include <fmi4cpp/tools/os_util.hpp>
#include <fmi4cpp/fmu_resource.hpp>

using namespace fmi4cpp;

fmu_resource::fmu_resource(fs::path &path): path_(path){}

const std::string fmu_resource::model_description_path() const {
    return path_.string() + "/modelDescription.xml";
}

const std::string fmu_resource::resource_path() const {
    return "file:///" + path_.string() + "/resources/" + getOs() + "/" + getLibExt();
}

const std::string fmu_resource::absolute_library_path(const std::string &modelIdentifier) const {
    return path_.string() + "/binaries/" + getOs() + "/" + modelIdentifier + getLibExt();
}

fmu_resource::~fmu_resource() {

    std::error_code success {};
    fs::remove_all(path_, success);

    if (!success) {
        FMI4CPP_DEBUG("Deleted temporal folder '" + path_.string() + "'");
    } else {
        FMI4CPP_DEBUG("Unable to delete temporal folder '" + path_.string() + "'");
    }

}
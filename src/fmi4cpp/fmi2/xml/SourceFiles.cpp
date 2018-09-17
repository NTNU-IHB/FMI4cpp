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
#include <fmi4cpp/fmi2/xml/SourceFiles.hpp>
#include <iostream>

using std::string;
using namespace fmi4cpp::fmi2::xml;

void File::load(const ptree &node) {
    name_ = node.get<string>("<xmlattr>.name");
}

std::string File::name() const {
    return name_;
}

const size_t SourceFiles::size() const {
    return vector::size();
}

File SourceFiles::at(const size_type index) const {
    return vector::at(index);
}

void SourceFiles::load(const ptree &node) {

    for (const ptree::value_type &v : node) {

        if (v.first == "File") {
            File file;
            file.load(v.second);
            push_back(file);
        }

    }

}

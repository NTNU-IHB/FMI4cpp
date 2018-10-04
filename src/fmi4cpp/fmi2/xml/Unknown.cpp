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

#include <sstream>
#include <iostream>
#include <fmi4cpp/fmi2/xml/Unknown.hpp>

using fmi4cpp::fmi2::xml::Unknown;

namespace {

    template <class T>
    std::optional<T> convert(boost::optional<T> opt) {
        if (!opt) {
            return {};
        } else {
            return *opt;
        }
    }

    void parseDependencies(const std::string &str, std::vector<unsigned int> &store) {
        int i;
        std::stringstream ss(str);
        while (ss >> i) {
            store.push_back(i);
            if (ss.peek() == ',' || ss.peek() == ' ') {
                ss.ignore();
            }
        }
    }

}

size_t Unknown::getIndex() const {
    return index_;
}

std::optional<std::string> Unknown::getDependenciesKind() const {
    return dependenciesKind_;
}

const std::optional<std::vector<unsigned int>> &Unknown::getDependencies() const {
    return dependencies_;
}

void Unknown::load(const ptree &node) {
    index_ = node.get<unsigned int>("<xmlattr>.index");
    dependenciesKind_ = convert(node.get_optional<std::string>("<xmlattr>.dependenciesKind"));

    auto dependencies = node.get_optional<std::string>("<xmlattr>.dependencies");
    if (dependencies) {
        dependencies_ = std::vector<unsigned int>();
        parseDependencies(*dependencies, *dependencies_);
    }

}
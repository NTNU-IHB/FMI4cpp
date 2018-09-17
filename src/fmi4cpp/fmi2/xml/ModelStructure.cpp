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
#include <boost/optional.hpp>
#include <fmi4cpp/fmi2/xml/ModelStructure.hpp>

using namespace std;
using namespace fmi4cpp::fmi2::xml;

namespace {

    void loadUnknowns(const ptree &node, vector<Unknown> &vector) {

        for (const ptree::value_type &v : node) {
            if (v.first == "Unknown") {
                Unknown unknown;
                unknown.load(v.second);
                vector.push_back(unknown);
            }
        }

    }

    template <class T>
    std::optional<T> convert(boost::optional<T> opt) {
        if (!opt) {
            return {};
        } else {
            return *opt;
        }
    }

    vector<unsigned int> parse(const string str) {
        int i;
        stringstream ss(str);
        vector<unsigned int> result;
        while (ss >> i) {
            result.push_back(i);
            if (ss.peek() == ',' || ss.peek() == ' ') {
                ss.ignore();
            }
        }
        return result;
    }

}

unsigned int Unknown::index() const {
    return index_;
}

std::optional<std::string> Unknown::dependencyKind() const {
    return dependencyKind_;
}

const std::optional<std::vector<unsigned int>> &Unknown::dependencies() const {
    return dependencies_;
}

void Unknown::load(const ptree &node) {
    index_ = node.get<unsigned int>("<xmlattr>.index");
    dependencyKind_ = convert(node.get_optional<string>("<xmlattr>.dependencyKind"));

    auto dependencies = node.get_optional<string>("<xmlattr>.dependencies");
    if (dependencies) {
        dependencies_ = parse(*dependencies);
    }

}


const std::vector<Unknown> &ModelStructure::outputs() const {
    return outputs_;
}

const std::vector<Unknown> &ModelStructure::derivatives() const {
    return derivatives_;
}

const std::vector<Unknown> &ModelStructure::initialUnknowns() const {
    return initialUnknowns_;
}

void ModelStructure::load(const ptree &node) {

    for (const ptree::value_type &v : node) {
        if (v.first == "Outputs") {
            loadUnknowns(v.second, outputs_);
        } else if (v.first == "Derivatives") {
            loadUnknowns(v.second, derivatives_);
        } else if (v.first == "InitialUnknowns") {
            loadUnknowns(v.second, initialUnknowns_);
        }
    }

}

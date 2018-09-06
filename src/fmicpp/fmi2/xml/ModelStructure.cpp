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

#include <boost/optional.hpp>
#include <fmicpp/fmi2/xml/ModelStructure.hpp>


using namespace std;
using namespace fmicpp::fmi2::xml;

namespace {

    void loadUnkowns(const ptree &node, vector<Unknown> &vector) {

        for (const ptree::value_type &v : node) {
            if (v.first == "Unknown") {
                Unknown unknown;
                unknown.load(v.second);
                vector.push_back(unknown);
            }
        }

    }

}

void Unknown::load(const ptree &node) {
    index = node.get<unsigned int>("<xmlattr>.index");
}

void ModelStructure::load(const ptree &node) {

    for (const ptree::value_type &v : node) {
        if (v.first == "Outputs") {
            loadUnkowns(v.second, outputs);
        } else if (v.first == "Derivatives") {
            loadUnkowns(v.second, derivatives);
        } else if (v.first == "InitialUnknowns") {
            loadUnkowns(v.second, initialUnknowns);
        }
    }

}

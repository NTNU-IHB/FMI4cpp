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

#include <fmicpp/fmi2/xml/ScalarVariableAttributes.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;

void IntegerAttribute::load(const ptree &node) {
    auto min_optional = node.get_optional<int>("<xmlattr>.min");
    if (min_optional) {
        min = make_shared<int>(*min_optional);
    }
    auto max_optional = node.get_optional<int>("<xmlattr>.max");
    if (max_optional) {
        max = make_shared<int>(*max_optional);
    }
    auto start_optional = node.get_optional<int>("<xmlattr>.start");
    if (start_optional) {
        start = make_shared<int>(*start_optional);
    }
    auto quantity_optional = node.get_optional<string>("<xmlattr>.quantity");
    if (quantity_optional) {
        quantity = make_shared<string>(*quantity_optional);
    }

}

void RealAttribute::load(const ptree &node) {
    auto min_optional = node.get_optional<double>("<xmlattr>.min");
    if (min_optional) {
        min = make_shared<double>(*min_optional);
    }
    auto max_optional = node.get_optional<double>("<xmlattr>.max");
    if (max_optional) {
        max = make_shared<double>(*max_optional);
    }
    auto start_optional = node.get_optional<double>("<xmlattr>.start");
    if (start_optional) {
        start = make_shared<double>(*start_optional);
    }
    auto nominal_optional = node.get_optional<double>("<xmlattr>.nominal");
    if (nominal_optional) {
        nominal = make_shared<double>(*nominal_optional);
    }

    auto quantity_optional = node.get_optional<string>("<xmlattr>.quantity");
    if (quantity_optional) {
        quantity = make_shared<string>(*quantity_optional);
    }
    auto unit_optional = node.get_optional<string>("<xmlattr>.unit");
    if (unit_optional) {
        unit = make_shared<string>(*unit_optional);
    }
    auto displayUnit_optional = node.get_optional<string>("<xmlattr>.displayUnit");
    if (displayUnit_optional) {
        displayUnit = make_shared<string>(*displayUnit_optional);
    }

    auto derivative_optional = node.get_optional<unsigned int>("<xmlattr>.derivative");
    if (derivative_optional) {
        derivative = make_shared<unsigned int>(*derivative_optional);
    }

    reinit = node.get<bool>("<xmlattr>.reinit", false);
    unbounded = node.get<bool>("<xmlattr>.unbounded", false);
    relativeQuantity = node.get<bool>("<xmlattr>.relativeQuantity", false);

}

void StringAttribute::load(const ptree &node) {
    auto start_optional = node.get_optional<string>("<xmlattr>.start");
    if (start_optional) {
        start = make_shared<string>(*start_optional);
    }
}

void BooleanAttribute::load(const ptree &node) {
    auto start_optional = node.get_optional<bool>("<xmlattr>.start");
    if (start_optional) {
        start = make_shared<bool>(*start_optional);
    }
}

void EnumerationAttribute::load(const ptree &node) {
    auto min_optional = node.get_optional<int>("<xmlattr>.min");
    if (min_optional) {
        min = make_shared<int>(*min_optional);
    }
    auto max_optional = node.get_optional<int>("<xmlattr>.max");
    if (max_optional) {
        max = make_shared<int>(*max_optional);
    }
    auto start_optional = node.get_optional<int>("<xmlattr>.start");
    if (start_optional) {
        start = make_shared<int>(*start_optional);
    }
    auto quantity_optional = node.get_optional<string>("<xmlattr>.quantity");
    if (quantity_optional) {
        quantity = make_shared<string>(*quantity_optional);
    }
}
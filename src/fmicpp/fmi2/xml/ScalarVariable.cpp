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

#include <fmicpp/fmi2/xml/ScalarVariable.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;

namespace {

    Causality parseCausality(string str) {
        if (str == "calculatedParameter") {
            return Causality::calculatedParameter;
        } else if (str == "parameter") {
            return Causality::parameter;
        } else if (str == "input") {
            return Causality::input;
        } else if (str == "output") {
            return Causality::output;
        } else if (str == "local") {
            return Causality::local;
        } else if (str == "independent") {
            return Causality::independent;
        } else {
            return Causality::local;
        }
    }

    Variability parseVariability(string str) {
        if (str == "constant") {
            return Variability::constant;
        } else if (str == "fixed") {
            return Variability::fixed;
        } else if (str == "tunable") {
            return Variability::tunable;
        } else if (str == "discrete") {
            return Variability::discrete;
        } else if (str == "continuous") {
            return Variability::continuous;
        } else {
            return Variability ::continuous;
        }
    }

}

void ScalarVariable::load(const ptree &node) {

    valueReference = node.get<fmi2ValueReference >("<xmlattr>.valueReference");
    name = node.get<string>("<xmlattr>.name");
    description = node.get<string>("<xmlattr>.description", "");
    canHandleMultipleSetPerTimelnstant = node.get<bool>("<xmlattr>.canHandleMultipleSetPerTimelnstant", false);

    causality = parseCausality(node.get<string >("<xmlattr>.causality", ""));
    variability = parseVariability(node.get<string >("<xmlattr>.variability", ""));

    for (const ptree::value_type &v : node) {
        if (v.first == "Integer") {
            integerAttribute = make_unique<IntegerAttribute>(IntegerAttribute());
            integerAttribute->load(v.second);
        } else if (v.first == "Real") {
            realAttribute = make_unique<RealAttribute>(RealAttribute());
            realAttribute->load(v.second);
        } else if (v.first == "String") {
            stringAttribute = make_unique<StringAttribute>(StringAttribute());
            stringAttribute->load(v.second);
        } else if (v.first == "Boolean") {
            booleanAttribute = make_unique<BooleanAttribute>(BooleanAttribute());
            booleanAttribute->load(v.second);
        } else if (v.first == "Enumeration") {
            enumerationAttribute = make_unique<EnumerationAttribute>(EnumerationAttribute());
            enumerationAttribute->load(v.second);
        }
    }

}

IntegerVariable ScalarVariable::asIntegerVariable() {
    return IntegerVariable(*integerAttribute);
}

RealVariable ScalarVariable::asRealVariable() {
    return RealVariable(*realAttribute);
}

StringVariable ScalarVariable::asStringVariable() {
    return StringVariable(*stringAttribute);
}

BooleanVariable ScalarVariable::asBooleanVariable() {
    return BooleanVariable(*booleanAttribute);
}

EnumerationVariable ScalarVariable::asEnumerationVariable() {
    return EnumerationVariable(*enumerationAttribute);
}

IntegerVariable::IntegerVariable(const IntegerAttribute &attr)
    : min(attr.min), max(attr.max), start(attr.start), quantity(attr.quantity) {};

RealVariable::RealVariable(const RealAttribute &attr)
    : min(attr.min), max(attr.max), start(attr.start), nominal(attr.nominal),
    reinit(attr.nominal), unbounded(attr.unbounded), relativeQuantity(attr.relativeQuantity),
    quantity(attr.quantity), unit(attr.unit), displayUnit(attr.displayUnit), derivative(attr.derivative) {};

StringVariable::StringVariable(const StringAttribute &attr)
    : start(attr.start) {};

BooleanVariable::BooleanVariable(const BooleanAttribute &attr)
    : start(attr.start) {};

EnumerationVariable::EnumerationVariable(const EnumerationAttribute &attr)
    : min(attr.min), max(attr.max), start(attr.start), quantity(attr.quantity) {};

//
// Created by laht on 30.08.18.
//

#include <fmicpp/fmi2/xml/ScalarVariable.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;

void IntegerAttribute::load(ptree &node) {

    min = node.get<int>("<xmlattr>.min", 0);
    max = node.get<int>("<xmlattr>.max", 0);
    start = node.get<int>("<xmlattr>.start", 0);

}

void RealAttribute::load(ptree &node) {

    min = node.get<double>("<xmlattr>.min", 0);
    max = node.get<double>("<xmlattr>.max", 0);
    start = node.get<double>("<xmlattr>.start", 0);

}

void StringAttribute::load(ptree &node) {

    start = node.get<string>("<xmlattr>.start", "");

}

void BooleanAttribute::load(ptree &node) {

    start = node.get<bool>("<xmlattr>.start", false);

}

void ScalarVariable::load(ptree &node) {

    valueReference = node.get<ValueReference>("<xmlattr>.valueReference");
    name = node.get<string>("<xmlattr>.name");
    description = node.get<string>("<xmlattr>.description", "");

    for (ptree::value_type &v : node) {
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
        }
    }

}

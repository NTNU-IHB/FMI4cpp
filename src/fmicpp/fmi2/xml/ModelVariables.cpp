//
// Created by laht on 30.08.18.
//

#include <fmicpp/fmi2/xml/ModelVariables.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;


void fmicpp::fmi2::xml::ModelVariables::load(ptree &node) {

    for (ptree::value_type &v : node) {

        if (v.first == "ScalarVariable") {
            unique_ptr<ScalarVariable> var(new ScalarVariable());
            var->load(v.second);
            push_back(move(var));
        }

    }

}

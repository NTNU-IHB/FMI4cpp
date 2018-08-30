//
// Created by laht on 30.08.18.
//

#ifndef FMICPP_MODELVARIABLES_HPP
#define FMICPP_MODELVARIABLES_HPP

#include <vector>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <fmicpp/fmi2/xml/ScalarVariable.hpp>

using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {

struct ModelVariables : public std::vector<std::unique_ptr<ScalarVariable>> {

        void load(ptree &node);

    };

}


#endif //FMICPP_MODELVARIABLES_HPP

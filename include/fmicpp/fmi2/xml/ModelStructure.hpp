//
// Created by laht on 30.08.18.
//

#ifndef FMICPP_MODELSTRUCTURE_HPP
#define FMICPP_MODELSTRUCTURE_HPP

#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;


namespace fmicpp::fmi2::xml {

    class ModelStructure {

        void load(ptree &node);

    };

}


#endif //FMICPP_MODELSTRUCTURE_HPP

//
// Created by laht on 30.08.18.
//

#ifndef FMICPP_SCALARVARIABLE_HPP
#define FMICPP_SCALARVARIABLE_HPP

#include <memory>
#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {
    
    typedef unsigned int ValueReference;

    struct IntegerAttribute {

        int min;
        int max;
        int start;

        void load(ptree &node);
        
    };

    struct RealAttribute {
        
        double min;
        double max;
        double start;
        
        void load(ptree &node);
        
    };

    struct StringAttribute {
        
        std::string start;

        void load(ptree &node);
        
    };

    struct BooleanAttribute {
        
        bool start;

        void load(ptree &node);

    };


    struct ScalarVariable {
        
        ValueReference valueReference;
        std::string name;
        std::string description;

        std::unique_ptr<IntegerAttribute> integerAttribute = nullptr;
        std::unique_ptr<RealAttribute> realAttribute = nullptr;
        std::unique_ptr<StringAttribute> stringAttribute = nullptr;
        std::unique_ptr<BooleanAttribute> booleanAttribute = nullptr;
        
        void load(ptree &node);

    };
    
}

#endif //FMICPP_SCALARVARIABLE_HPP

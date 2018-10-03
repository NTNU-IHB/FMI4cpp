//
// Created by LarsIvar on 02.10.2018.
//

#ifndef FMI4CPP_UNKNOWN_HPP
#define FMI4CPP_UNKNOWN_HPP

#include <vector>
#include <string>
#include <optional>

#include <boost/property_tree/ptree.hpp>

using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    class Unknown {

    private:
        size_t index_;
        std::optional<std::string> dependenciesKind_;
        std::optional<std::vector<unsigned int >> dependencies_;

    public:
        size_t getIndex() const;

        std::optional<std::string> getDependenciesKind() const;

        const std::optional<std::vector<unsigned int>> &getDependencies() const;

        void load(const ptree &node);

    };

}


#endif //FMI4CPP_UNKNOWN_HPP

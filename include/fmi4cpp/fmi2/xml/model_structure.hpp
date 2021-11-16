
#ifndef FMI4CPP_MODELSTRUCTURE_HPP
#define FMI4CPP_MODELSTRUCTURE_HPP

#include <string>
#include <vector>
#include <optional>

namespace fmi4cpp::fmi2
{

struct unknown
{
    unsigned int index;
    std::optional<std::vector<unsigned int>> dependencies;
    std::optional<std::vector<std::string>> dependencies_kind;
};

struct model_structure
{
    const std::vector<unknown> outputs;
    const std::vector<unknown> derivatives;
    const std::vector<unknown> initialUnknowns;

    model_structure(std::vector<unknown> outputs, std::vector<unknown> derivatives,
        std::vector<unknown> initialUnknowns)
        : outputs(std::move(outputs))
        , derivatives(std::move(derivatives))
        , initialUnknowns(std::move(initialUnknowns))
    {}
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_MODELSTRUCTURE_HPP

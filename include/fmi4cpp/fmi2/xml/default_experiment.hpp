
#ifndef FMI4CPP_DEFAULTEXPERIMENT_HPP
#define FMI4CPP_DEFAULTEXPERIMENT_HPP

#include <optional>

namespace fmi4cpp::fmi2
{

struct default_experiment
{
    std::optional<double> startTime;
    std::optional<double> stopTime;
    std::optional<double> stepSize;
    std::optional<double> tolerance;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_DEFAULTEXPERIMENT_HPP

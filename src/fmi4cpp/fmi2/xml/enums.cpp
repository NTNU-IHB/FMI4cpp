
#include <fmi4cpp/fmi2/xml/enums.hpp>

using fmi4cpp::fmi2::causality;
using fmi4cpp::fmi2::initial;
using fmi4cpp::fmi2::variability;

causality fmi4cpp::fmi2::parse_causality(const std::string& str)
{
    if (str == "calculatedParameter") {
        return causality::calculatedParameter;
    } else if (str == "parameter") {
        return causality::parameter;
    } else if (str == "input") {
        return causality::input;
    } else if (str == "output") {
        return causality::output;
    } else if (str == "independent") {
        return causality::independent;
    } else {
        return causality::local;
    }
}

variability fmi4cpp::fmi2::parse_variability(const std::string& str)
{
    if (str == "constant") {
        return variability::constant;
    } else if (str == "fixed") {
        return variability::fixed;
    } else if (str == "tunable") {
        return variability::tunable;
    } else if (str == "discrete") {
        return variability::discrete;
    } else {
        return variability::continuous;
    }
}

initial fmi4cpp::fmi2::parse_initial(const std::string& str)
{
    if (str == "exact") {
        return initial::exact;
    } else if (str == "approx") {
        return initial::approx;
    } else if (str == "calculated") {
        return initial::calculated;
    } else {
        return initial::unknown;
    }
}

std::string fmi4cpp::fmi2::to_string(causality causality)
{
    switch (causality) {
        case causality::calculatedParameter:
            return "calculatedParameter";
        case causality::parameter:
            return "parameter";
        case causality::input:
            return "input";
        case causality::output:
            return "output";
        case causality::local:
            return "local";
        case causality::independent:
            return "independent";
        case causality::unknown:
        default:
            return "unknown";
    }
}

std::string fmi4cpp::fmi2::to_string(variability variability)
{
    switch (variability) {
        case variability::constant:
            return "constant";
        case variability::fixed:
            return "fixed";
        case variability::tunable:
            return "tunable";
        case variability::discrete:
            return "discrete";
        case variability::continuous:
            return "continuous";
        case variability::unknown:
        default:
            return "unknown";
    }
}

std::string fmi4cpp::fmi2::to_string(initial initial)
{
    switch (initial) {
        case initial::exact:
            return "exact";
        case initial::approx:
            return "approx";
        case initial::calculated:
            return "calculated";
        case initial::unknown:
        default:
            return "unknown";
    }
}

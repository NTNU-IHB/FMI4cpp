/*
 * File Name:    unit_definitions.h
 * Description:  Defines structures for handling physical units and their conversions, including base units, display units, and unit definitions.
 * Author:       weilong.wen
 * Email:       weilong.wen.mail@gmail.com
 * Created:     2025-02-03
 */

#ifndef FMI4CPP_UNITDEFINITIONS_HPP
#define FMI4CPP_UNITDEFINITIONS_HPP

#include <optional>
#include <string>
#include <vector>

namespace fmi4cpp::fmi2
{

struct base_unit
{
    std::optional<int> kg = 0;
    std::optional<int> m = 0;
    std::optional<int> s = 0;
    std::optional<int> A = 0;
    std::optional<int> K = 0;
    std::optional<int> mol = 0;
    std::optional<int> cd = 0;
    std::optional<int> rad = 0;
    std::optional<double> factor = 1.0;
    std::optional<double> offset = 0.0;
};

struct display_unit
{
    std::string name;
    double factor = 1.0;
    double offset = 0.0;
};

struct unit
{
    std::string name;
    ::fmi4cpp::fmi2::base_unit base_unit;
    std::vector<display_unit> display_units;
};

struct unit_definitions
{
    std::vector<unit> units;
};

} // namespace fmi4cpp::fmi2

#endif // FMI4CPP_UNITDEFINITIONS_HPP

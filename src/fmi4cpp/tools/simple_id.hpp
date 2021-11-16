
#ifndef FMI4CPP_SIMPLE_ID_HPP
#define FMI4CPP_SIMPLE_ID_HPP

#include <random>
#include <string>

namespace
{

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(0, 9);

std::string generate_simple_id(const unsigned int len)
{
    std::string id;
    for (unsigned int i = 0; i < len; i++) {
        id += std::to_string(dist(mt));
    }
    return id;
}

} // namespace

#endif //FMI4CPP_SIMPLE_ID_HPP

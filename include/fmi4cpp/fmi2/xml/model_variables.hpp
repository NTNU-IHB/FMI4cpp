
#ifndef FMI4CPP_MODELVARIABLES_HPP
#define FMI4CPP_MODELVARIABLES_HPP

#include <fmi4cpp/fmi2/xml/scalar_variable.hpp>

#include <memory>
#include <string>
#include <vector>

namespace fmi4cpp::fmi2
{

class model_variables
{

private:
    const std::vector<scalar_variable> variables_ = {};

public:
    model_variables();

    explicit model_variables(std::vector<scalar_variable>  variables);

    [[nodiscard]] size_t size() const;

    const scalar_variable& operator[](size_t index) const;
    [[nodiscard]] const scalar_variable& getByName(const std::string& name) const;
    [[nodiscard]] const scalar_variable& getByValueReference(fmi2ValueReference vr) const;

    void getByValueReference(fmi2ValueReference vr, std::vector<scalar_variable>& store) const;
    void getByCausality(causality causality, std::vector<scalar_variable>& store) const;

    [[nodiscard]] std::vector<scalar_variable>::const_iterator begin() const;
    [[nodiscard]] std::vector<scalar_variable>::const_iterator end() const;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_MODELVARIABLES_HPP

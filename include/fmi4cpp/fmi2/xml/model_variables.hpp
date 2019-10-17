/*
 * The MIT License
 *
 * Copyright 2017-2018 Norwegian University of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING  FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
    const std::vector<scalar_variable> variables_;

public:
    model_variables();

    explicit model_variables(const std::vector<scalar_variable>& variables);

    size_t size() const;

    const scalar_variable& operator[](size_t index) const;
    const scalar_variable& getByName(const std::string& name) const;
    const scalar_variable& getByValueReference(fmi2ValueReference vr) const;

    void getByValueReference(fmi2ValueReference vr, std::vector<scalar_variable>& store) const;
    void getByCausality(causality causality, std::vector<scalar_variable>& store) const;

    std::vector<scalar_variable>::const_iterator begin() const;
    std::vector<scalar_variable>::const_iterator end() const;
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_MODELVARIABLES_HPP

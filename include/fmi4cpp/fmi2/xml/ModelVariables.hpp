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


#include <vector>
#include <memory>

#include <fmi4cpp/fmi2/xml/ScalarVariable.hpp>

namespace fmi4cpp::fmi2 {

    class ModelVariables {

    private:

        std::vector<ScalarVariable> variables_;

    public:

        ModelVariables();

        explicit ModelVariables(const std::vector<ScalarVariable> &variables);

        size_t size() const;
        
        const std::vector<ScalarVariable> variables() const;

        const ScalarVariable &operator[](size_t index) const;
        const ScalarVariable &getByName(const std::string &name) const;
        const ScalarVariable &getByValueReference(fmi2ValueReference vr) const;

        void getByValueReference(fmi2ValueReference vr, std::vector<ScalarVariable> &store) const;
        void getByCausality(fmi2Causality causality, std::vector<ScalarVariable> &store) const;

        std::vector<ScalarVariable>::iterator begin();
        std::vector<ScalarVariable>::iterator end();

        std::vector<ScalarVariable>::const_iterator begin() const;
        std::vector<ScalarVariable>::const_iterator end() const;

    };

}

#endif //FMI4CPP_MODELVARIABLES_HPP

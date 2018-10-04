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
#include <boost/property_tree/ptree.hpp>
#include <fmi4cpp/fmi2/xml/ScalarVariable.hpp>

using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    struct ModelVariables {

        virtual const size_t size() const = 0;

        virtual const ScalarVariable &operator[](size_t index) const = 0;

        virtual const ScalarVariable &getByName(const std::string &name) const = 0;

        virtual const ScalarVariable &getByValueReference(fmi2ValueReference vr) const = 0;

        virtual void getByCausality(fmi2Causality causality, std::vector<std::reference_wrapper<ScalarVariable>> &store) const = 0;

        virtual std::vector<std::shared_ptr<ScalarVariable>>::iterator begin() = 0;

        virtual std::vector<std::shared_ptr<ScalarVariable>>::iterator end() = 0;

        virtual std::vector<std::shared_ptr<ScalarVariable>>::const_iterator cbegin() const = 0;

        virtual std::vector<std::shared_ptr<ScalarVariable>>::const_iterator cend() const = 0;
    };


    class ModelVariablesImpl: public virtual ModelVariables {

    private:
        std::vector<std::shared_ptr<ScalarVariable>> variables_;

    public:

        explicit ModelVariablesImpl(const ptree &node);

        const ScalarVariable &getByName(const std::string &name) const override;

        const ScalarVariable &getByValueReference(fmi2ValueReference vr) const override;

        void getByCausality(fmi2Causality causality, std::vector<std::reference_wrapper<ScalarVariable>> &store) const override;

        const size_t size() const override;

        const ScalarVariable &operator[](size_t index) const override;

        std::vector<std::shared_ptr<ScalarVariable>>::iterator begin() override;

        std::vector<std::shared_ptr<ScalarVariable>>::iterator end() override;

        std::vector<std::shared_ptr<ScalarVariable>>::const_iterator cbegin() const override;

        std::vector<std::shared_ptr<ScalarVariable>>::const_iterator cend() const override;

    };

}

#endif //FMI4CPP_MODELVARIABLES_HPP

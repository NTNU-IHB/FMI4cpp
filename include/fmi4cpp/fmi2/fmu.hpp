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

#ifndef FMI4CPP_FMI2FMU_HPP
#define FMI4CPP_FMI2FMU_HPP

#include <fmi4cpp/fmi2/cs_fmu.hpp>
#include <fmi4cpp/fmi2/me_fmu.hpp>
#include <fmi4cpp/fmi2/xml/cs_model_description.hpp>
#include <fmi4cpp/fmi2/xml/me_model_description.hpp>
#include <fmi4cpp/fmu_base.hpp>

#include <memory>
#include <string>

namespace fmi4cpp::fmi2
{

class fmu : public virtual fmu_provider<model_description, cs_fmu, me_fmu>
{

    friend class cs_fmu;
    friend class me_fmu;

private:
    std::shared_ptr<fmu_resource> resource_;
    std::shared_ptr<const fmi4cpp::fmi2::model_description> modelDescription_;

public:
    explicit fmu(const std::string& fmuPath);

    [[nodiscard]] std::string get_model_description_xml() const;
    [[nodiscard]] std::shared_ptr<const fmi4cpp::fmi2::model_description> get_model_description() const override;

    [[nodiscard]] bool supports_me() const override;
    [[nodiscard]] bool supports_cs() const override;

    [[nodiscard]] std::unique_ptr<cs_fmu> as_cs_fmu() const override;
    [[nodiscard]] std::unique_ptr<me_fmu> as_me_fmu() const override;
};

} // namespace fmi4cpp::fmi2


#endif //FMI4CPP_FMI2FMU_HPP

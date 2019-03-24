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

#ifndef FMI4CPP_FMI2MODELEXCHANGEFMU_H
#define FMI4CPP_FMI2MODELEXCHANGEFMU_H

#include <fmi4cpp/fmu_base.hpp>
#include <fmi4cpp/fmu_resource.hpp>

#include <fmi4cpp/fmi2/me_slave.hpp>
#include <fmi4cpp/fmi2/xml/me_model_description.hpp>

namespace fmi4cpp::fmi2 {

    class me_fmu : public virtual me_fmu_base<me_instance, me_slave, me_model_description> {

    private:

        std::shared_ptr<fmu_resource> resource_;
        std::shared_ptr<me_library> lib_;
        std::shared_ptr<const me_model_description> modelDescription_;

    public:

        me_fmu(const std::shared_ptr<fmu_resource> &resource,
                             const std::shared_ptr<const me_model_description> &md);

        std::shared_ptr<const me_model_description> get_model_description() const override;

        std::unique_ptr<me_instance> new_instance(bool visible = false, bool loggingOn = false);

        std::unique_ptr<me_slave>
        new_instance(std::unique_ptr<fmi4cpp::solver::me_solver> &solver, bool visible = false, bool loggingOn = false);

    };

}

#endif //FMI4CPP_FMI2MODELEXCHANGEFMU_H

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

#ifndef FMI4CPP_FMU_HPP
#define FMI4CPP_FMU_HPP

#include <memory>
#include <string>

#include <fmi4cpp/solver/solver.hpp>

namespace fmi4cpp {

    template<class ModelDescription>
    class fmu_base {

    public:

        const std::string guid() const {
            return get_model_description()->guid;
        }

        const std::string model_name() const {
            return get_model_description()->model_name;
        }

        virtual std::shared_ptr<const ModelDescription> get_model_description() const = 0;

    };

    template<class model_description, class cs_fmu, class me_fmu>
    class fmu_provider : public virtual fmu_base<model_description> {

    public:

        virtual bool supports_cs() const = 0;

        virtual bool supports_me() const = 0;

        virtual std::unique_ptr <cs_fmu> as_cs_fmu() const = 0;

        virtual std::unique_ptr <me_fmu> as_me_fmu() const = 0;

    };

    template <class cs_slave, class cs_model_description>
    class cs_fmu_base : public fmu_base<cs_model_description> {

    public:

        virtual std::shared_ptr<const cs_model_description> get_model_description() const = 0;

        virtual std::unique_ptr<cs_slave> new_instance(bool visible, bool loggingOn) = 0;

    };

    template <class me_instance, class me_slave, class me_model_description>
    class me_fmu_base : public fmu_base<me_model_description> {

    public:

        virtual std::shared_ptr<const me_model_description> get_model_description() const override = 0;

        virtual std::unique_ptr<me_instance> new_instance(bool visible = false, bool loggingOn = false) = 0;

        virtual std::unique_ptr<me_slave>
        new_instance(std::unique_ptr<fmi4cpp::solver::me_solver> &solver, bool visible = false, bool loggingOn = false) = 0;

    };

}


#endif //FMI4CPP_FMU_HPP

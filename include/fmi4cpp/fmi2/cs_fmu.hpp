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

#ifndef FMI4CPP_FMI2COSIMULATIONFMU_H
#define FMI4CPP_FMI2COSIMULATIONFMU_H

#include <fmi4cpp/fmu_base.hpp>
#include <fmi4cpp/fmu_resource.hpp>

#include <fmi4cpp/fmi2/cs_library.hpp>
#include <fmi4cpp/fmi2/cs_slave.hpp>
#include <fmi4cpp/fmi2/xml/cs_model_description.hpp>


namespace fmi4cpp::fmi2 {
    
    class cs_fmu : public cs_fmu_base<cs_slave, cs_model_description> {

    private:

        std::shared_ptr<cs_library> lib_;
        std::shared_ptr<fmu_resource> resource_;
        std::shared_ptr<const cs_model_description> modelDescription_;

    public:

        cs_fmu(const std::shared_ptr<fmu_resource> &resource,
                            const std::shared_ptr<const cs_model_description> &md);

        std::shared_ptr<const cs_model_description> model_description() const override;

        std::unique_ptr<cs_slave> new_instance(bool visible = false, bool loggingOn = false);

    };

}

#endif //FMI4CPP_FMI2COSIMULATIONFMU_H

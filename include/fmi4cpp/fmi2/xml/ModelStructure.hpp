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

#ifndef FMI4CPP_MODELSTRUCTURE_HPP
#define FMI4CPP_MODELSTRUCTURE_HPP

#include <vector>
#include <boost/property_tree/ptree.hpp>

#include "Unknown.hpp"
#include "../fmi2TypesPlatform.h"

using boost::property_tree::ptree;

namespace fmi4cpp::fmi2::xml {

    class ModelStructure {

    public:

        virtual const std::vector<Unknown> &getOutputs() const = 0;

        virtual const std::vector<Unknown> &getDerivatives() const = 0;

        virtual const std::vector<Unknown> &getInitialUnknowns() const = 0;

    };

    class ModelStructureImpl: public virtual ModelStructure {

    private:
        std::vector<Unknown> outputs_;
        std::vector<Unknown> derivatives_;
        std::vector<Unknown> initialUnknowns_;

    public:

        explicit ModelStructureImpl(const ptree &node);

        const std::vector<Unknown> &getOutputs() const override;

        const std::vector<Unknown> &getDerivatives() const override;

        const std::vector<Unknown> &getInitialUnknowns() const override;

    };

}

#endif //FMI4CPP_MODELSTRUCTURE_HPP

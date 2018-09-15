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

#ifndef FMICPP_MODELSTRUCTURE_HPP
#define FMICPP_MODELSTRUCTURE_HPP

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>

#include "../fmi2TypesPlatform.h"

using boost::property_tree::ptree;

namespace fmicpp::fmi2::xml {

    class Unknown {

    private:
        unsigned int index_;
        boost::optional<std::string> dependencyKind_;
        boost::optional<std::vector<unsigned int >> dependencies_;

    public:
        unsigned int index() const;

        boost::optional<std::string> dependencyKind() const;

        const boost::optional<std::vector<unsigned int>> &dependencies() const;

        void load(const ptree &node);

    };

    class ModelStructure {

    private:
        std::vector<Unknown> outputs_;
        std::vector<Unknown> derivatives_;
        std::vector<Unknown> initialUnknowns_;

    public:

        const std::vector<Unknown> &outputs() const;

        const std::vector<Unknown> &derivatives() const;

        const std::vector<Unknown> &initialUnknowns() const;

        void load(const ptree &node);

    };

}

#endif //FMICPP_MODELSTRUCTURE_HPP

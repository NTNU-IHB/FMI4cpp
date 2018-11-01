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

#ifndef FMI4CPP_SOLVER_HPP
#define FMI4CPP_SOLVER_HPP

#include <vector>
#include <functional>

#include <boost/numeric/odeint.hpp>

using namespace boost::numeric::odeint;

namespace fmi4cpp::fmi2 {

    class ModelExchangeSlave;
    class sys_wrapper;

    class Solver {

    public:

        virtual void integrate(sys_wrapper &sys, std::vector<double> &x, double tStart, double tStop) = 0;

        virtual ~Solver() = default;

    };


    class RK4Solver: public Solver {

    private:
        double stepSize_;
        runge_kutta4<std::vector<double>> stepper_;

    public:

        explicit RK4Solver(double stepSize);

        void integrate(sys_wrapper &sys, std::vector<double> &x, double tStart, double tStop) override;

    };



}

#endif //FMI4CPP_SOLVER_HPP

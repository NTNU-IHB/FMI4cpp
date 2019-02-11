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

#ifndef FMI4CPP_ODEINTSOLVER_HPP
#define FMI4CPP_ODEINTSOLVER_HPP

#include <boost/numeric/odeint/stepper/euler.hpp>
#include <boost/numeric/odeint/stepper/implicit_euler.hpp>

#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4_classic.hpp>

#include <boost/numeric/odeint/integrate/integrate_const.hpp>

#include "ModelExchangeSolver.hpp"

using namespace boost::numeric::odeint;

namespace fmi4cpp::solver {

    template <class T>
    class OdeintModelExchangeSolver: public ModelExchangeSolver {

    protected:

        T solver_;

    };

    template <class T>
    class ConstantStepSizeOdeintSolver: public OdeintModelExchangeSolver<T> {

    protected:

        double stepSize_;

    public:

        explicit ConstantStepSizeOdeintSolver(double stepSize): stepSize_(stepSize) {}

        double integrate(FmuWrapper &wrapper, state_type &x, double tStart, double tStop) override {
            size_t n = integrate_const(this->solver_, wrapper, x, tStart, tStop, stepSize_);
            return tStart + n*stepSize_;
        }

    };

    typedef ConstantStepSizeOdeintSolver<euler<state_type>> EulerSolver;
    typedef ConstantStepSizeOdeintSolver<implicit_euler<state_type>> ImplicitEulerSolver;

    typedef ConstantStepSizeOdeintSolver<runge_kutta4<state_type>> RK4Solver;
    typedef ConstantStepSizeOdeintSolver<runge_kutta4_classic<state_type>> RK4ClassicSolver;

}

#endif //FMI4CPP_ODEINTSOLVER_HPP

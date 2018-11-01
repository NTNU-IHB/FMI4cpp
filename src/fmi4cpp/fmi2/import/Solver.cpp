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

#include <utility>
#include <iostream>

#include <fmi4cpp/fmi2/import/ModelExchangeSlave.hpp>
#include <fmi4cpp/fmi2/import/Solver.hpp>

using namespace fmi4cpp::fmi2;

EulerSolver::EulerSolver(double stepSize): stepSize_(stepSize) {}

void EulerSolver::integrate(sys_wrapper &sys, std::vector<double> &x, double tStart, double tStop) {
    ::integrate(sys, x, tStart, tStop, stepSize_);
}

RK4Solver::RK4Solver(double stepSize): stepSize_(stepSize) {}

void RK4Solver::integrate(sys_wrapper &sys, std::vector<double> &x, double tStart, double tStop) {
    ::integrate(sys, x, tStart, tStop, stepSize_);
}

RK4ClassicSolver::RK4ClassicSolver(double stepSize): stepSize_(stepSize) {}

void RK4ClassicSolver::integrate(sys_wrapper &sys, std::vector<double> &x, double tStart, double tStop) {
    ::integrate(sys, x, tStart, tStop, stepSize_);
}



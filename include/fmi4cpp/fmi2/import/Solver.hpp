//
// Created by LarsIvar on 13.10.2018.
//

#ifndef FMI4CPP_SOLVER_HPP
#define FMI4CPP_SOLVER_HPP

#include <vector>

namespace fmi4cpp::fmi2::import {

    class Solver {

        virtual double integrate(double t0, std::vector<double> &x0, double t, std::vector<double> &x) = 0;

        virtual ~Solver() = default;

    };

}

#endif //FMI4CPP_SOLVER_HPP

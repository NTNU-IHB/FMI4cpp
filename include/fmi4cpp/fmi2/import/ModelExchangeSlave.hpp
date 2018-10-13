//
// Created by LarsIvar on 13.10.2018.
//

#ifndef FMI4CPP_MODELEXCHANGESLAVE_HPP
#define FMI4CPP_MODELEXCHANGESLAVE_HPP

#include <memory>

#include "ModelExchangeInstance.hpp"
#include "FmuSlave.hpp"
#include "Solver.hpp"

namespace fmi4cpp::fmi2::import {

    class ModelExchangeSlave : public FmuSlave {

    public:
        ModelExchangeSlave(std::unique_ptr<ModelExchangeInstance> &instance_,
                           std::unique_ptr<Solver> &solver);



    private:

        std::unique_ptr<Solver> solver_;
        std::unique_ptr<ModelExchangeInstance> instance_;

    };

}


#endif //FMI4CPP_MODELEXCHANGESLAVE_HPP

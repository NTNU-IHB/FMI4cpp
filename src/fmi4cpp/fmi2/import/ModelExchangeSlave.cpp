//
// Created by LarsIvar on 13.10.2018.
//

#include <fmi4cpp/fmi2/import/ModelExchangeSlave.hpp>

using namespace fmi4cpp::fmi2::import;

fmi4cpp::fmi2::import::ModelExchangeSlave::ModelExchangeSlave(
        std::unique_ptr<import::ModelExchangeInstance> &instance,
        std::unique_ptr<Solver> &solver)
        : instance_(std::move(instance)), solver_(std::move(solver)) {}

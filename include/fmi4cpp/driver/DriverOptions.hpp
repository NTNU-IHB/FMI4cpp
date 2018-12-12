//
// Created by laht on 12.12.18.
//

#ifndef FMI4CPP_DRIVEROPTIONS_HPP
#define FMI4CPP_DRIVEROPTIONS_HPP

namespace fmi4cpp::driver {

    struct DriverOptions {

        double startTime = 0.0;
        double stopTime = 0.0;
        double stepSize = 1e-3;

        bool modelExchange = false;
        bool failOnLargeFileSize = false;

        fs::path outputFolder = fs::current_path();
        std::vector<fmi4cpp::fmi2::ScalarVariable> variables;

    };
    
}

#endif //FMI4CPP_DRIVEROPTIONS_HPP

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

#include <fmi4cpp/driver/fmu_driver.hpp>
#include <fmi4cpp/fmi2/fmi2.hpp>

#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

using namespace fmi4cpp::fmi2;
using namespace fmi4cpp::driver;

namespace fs = std::filesystem;

namespace
{

const int SUCCESS = 0;
const int COMMANDLINE_ERROR = 1;
const int UNHANDLED_ERROR = 2;

const char* START = "start";
const char* STOP = "stop";
const char* STEP_SIZE = "stepsize";

} // namespace


int main(int argc, char** argv)
{

    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()("help,h", "Print this help message and quits.")("me", "Treat FMU as an Model Exchange FMU.")("fmu,f", po::value<string>(), "Path to FMU.")("output,o", po::value<string>(), "Where to store the generated CSV results.")(START, po::value<double>(), "Start time.")(STOP, po::value<double>(), "Stop time.")(STEP_SIZE, po::value<double>(), "StepSize.")("variables,v", po::value<vector<string>>()->multitoken(), "Variables to print.");

    if (argc == 1) {
        cout << "fmu_driver" << endl
             << desc << endl;
        return 0;
    }

    po::variables_map vm;
    try {

        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            cout << "fmu_driver" << endl
                 << desc << endl;
            return SUCCESS;
        }

        po::notify(vm);

    } catch (po::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl
                  << std::endl;
        std::cerr << desc << std::endl;
        return COMMANDLINE_ERROR;
    }

    if (!vm.count("fmu")) {
        cout << "Missing path to FMU.. Please try again." << endl;
        cout << "fmu_driver" << endl
             << desc << endl;
        return COMMANDLINE_ERROR;
    }

    const string fmuPath = vm["fmu"].as<string>();

    if (!vm.count("variables")) {
        cerr << "Missing variables to print.. Please try again." << endl;
        cout << "fmu_driver" << endl
             << desc << endl;
    }

    driver_options options;

    options.variables = vm["variables"].as<vector<string>>();

    if (vm.count(START)) {
        options.startTime = vm[START].as<double>();
    }
    if (vm.count(STOP)) {
        options.stopTime = vm[STOP].as<double>();
    }
    if (vm.count(STEP_SIZE)) {
        options.stepSize = vm[STEP_SIZE].as<double>();
    }

    if (vm.count("output")) {
        options.outputFolder = fs::path(vm["output"].as<string>());
    }

    if (vm.count("me")) {
        options.modelExchange = true;
    }

    fmu_driver driver(fmuPath, options);
    driver.run();

    return 0;
}

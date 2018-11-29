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

#include <memory>
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>

#include <fmi4cpp/fmi2/fmi4cpp.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

namespace {

    const int SUCCESS = 0;
    const int COMMANDLINE_ERROR = 1;
    const int UNHANDLED_ERROR = 2;

}

int main(int argc, char** argv) {

    namespace po = boost::program_options;

    po::options_description desc("Options");
    desc.add_options()
            ("help,h", "Print this help message and quits.")
            ("fmu,f", po::value<string>(), "Path to FMU.")
            ("startTime,start", po::value<string>(), "Start time.")
            ("stopTime,stop", po::value<string>(), "Stop time.")
            ("stepSize,dt", po::value<string>(), "StepSize.");

    if (argc == 1) {
        cout << "fmudriver" << endl << desc << endl;
    }

    po::variables_map vm;
    try {

        po::store(po::parse_command_line(argc, argv, desc), vm);

        if ( vm.count("help") ) {
            cout << "fmudriver" << endl << desc << endl;
            return SUCCESS;
        }

        po::notify(vm);

    } catch(po::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        return COMMANDLINE_ERROR;
    }

    return 0;

}

struct DriverOptions {

    double startTime = 0.0;
    double stopTime = 0.0;
    double stepSize = 1e-3;

    bool modelExchange = false;

    string outputFolder;
    vector<string> variables;

};

class FmuDriver {


public:


    FmuDriver(const string &fmuPath, DriverOptions &options) : fmuPath(fmuPath), options(options) {

        for (const auto v : )

    }

    void run() {

        if (options.modelExchange) {
            auto solver = make_solver<RK4ClassicSolver>(1E-3);
            simulate(Fmu(fmuPath).asModelExchangeFmu()->newInstance(solver));
        } else {
            simulate(Fmu(fmuPath).asCoSimulationFmu()->newInstance());
        }

    }

private:
    const string fmuPath;
    const DriverOptions options;

    vector<ScalarVariable> variables;

    void addHeader(string &data) {

        data += "\"Time\"";

        auto variables = options.variables;
        for (unsigned long i = 0; i < variables.size(); i++) {
            data += "\"" + variables[i] + "\"";
            if (i != variables.size()-1) {
                data += ",";
            }
        }
        data += "\n";

    }
    void addRow(FmuSlave &slave, string &data) {

        data += to_string(slave.getSimulationTime());
        auto variables = options.variables;
        for (unsigned long i = 0; i < variables.size(); i++) {
            data += "\"" + variables[i] + "\"";
            if (i != variables.size()-1) {
                data += ",";
            }
        }
        data += "\n";

    }

    void simulate(unique_ptr<FmuSlave> slave) {

        auto startTime = options.startTime;
        auto stopTime = options.stopTime;
        auto stepSize = options.stepSize;

        slave->setupExperiment(startTime);
        slave->enterInitializationMode();
        slave->exitInitializationMode();

        double t;
        string data = "";
        addHeader(data);
        while ( (t = slave->getSimulationTime()) <= (stopTime - stepSize) ) {

            addRow(*slave, data);

            if (!slave->doStep(stepSize)) {
                break;
            }

        }

        slave->terminate();

    }


};
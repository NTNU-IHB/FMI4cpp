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

#include <fstream>
#include <vector>
#include <iostream>

#include <fmi4cpp/common/driver/fmu_driver.hpp>

using namespace std;
using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

using namespace fmi4cpp::driver;

namespace {

    const char *CSV_SEPARATOR = ",";

    void addHeader(vector<ScalarVariable> &variables, std::string &data) {

        data += "\"Time\", ";

        for (unsigned long i = 0; i < variables.size(); i++) {
            data += "\"" + variables[i].name + "\"";
            if (i != variables.size() - 1) {
                data += CSV_SEPARATOR;
            }
        }

    }

    void addRow(FmuSlave<CoSimulationModelDescription> &slave, vector<ScalarVariable> &variables, string &data) {

        data += "\n" + to_string(slave.getSimulationTime()) + CSV_SEPARATOR;
        for (unsigned int i = 0; i < variables.size(); i++) {
            auto var = variables[i];

            if (var.isInteger()) {
                int ref = 0;
                slave.readInteger(var.valueReference, ref);
                data += to_string(ref);
            } else if (var.isReal()) {
                double ref = 0;
                slave.readReal(var.valueReference, ref);
                data += to_string(ref);
            } else if (var.isString()) {
                const char *ref;
                slave.readString(var.valueReference, ref);
                data += ref;
            } else if (var.isBoolean()) {
                int ref = 0;
                slave.readBoolean(var.valueReference, ref);
                data += to_string(ref);
            }

            if (i != variables.size() - 1) {
                data += CSV_SEPARATOR;
            }
        }

    }

}

fmu_driver::fmu_driver(const std::string &fmuPath, const driver_options &options) : fmuPath_(fmuPath), options_(options){}

void fmu_driver::run() {

    fmi2::fmi2Fmu fmu(fmuPath_);

    if (fmu.getModelDescription()->asCoSimulationModelDescription()->needsExecutionTool) {
        throw Rejection("FMU requires execution tool.");
    }

    if (options_.modelExchange) {
#ifdef FMI4CPP_WITH_ODEINT
        auto solver = solver::make_solver<solver::EulerSolver>(1E-3);
        simulate(fmu.asModelExchangeFmu()->newInstance(solver));
#else
        const char *msg = "Model Exchange mode selected, but driver has been built without odeint support!";
        throw Failure(msg);
#endif
    } else {
        simulate(fmu.asCoSimulationFmu()->newInstance());
    }

}

void fmu_driver::dumpOutput(const string &data, const string &outputFolder) {

    const auto fmuName = fs::path(fmuPath_).stem().string();
    const auto outputFile = fs::path(outputFolder + "/" + fmuName + "_out.csv");
    fs::create_directories(outputFile.parent_path());

    ofstream out(outputFile.string(), ofstream::out);
    out << data;
    out.flush();
    out.close();

}

void fmu_driver::simulate(std::unique_ptr<FmuSlave<CoSimulationModelDescription>> slave) {

    auto startTime = options_.startTime;
    auto stopTime = options_.stopTime;
    auto stepSize = options_.stepSize;

    slave->setupExperiment(startTime);
    slave->enterInitializationMode();
    slave->exitInitializationMode();

    string data;
    vector<ScalarVariable> variables = options_.transformVariables(slave->getModelDescription());

    addHeader(variables, data);

    addRow(*slave, variables, data);
    while (slave->getSimulationTime() <= stopTime) {

        if (!slave->doStep(stepSize)) {
            slave->terminate();
            throw Failure("Simulation terminated prematurely.");
        }

        addRow(*slave, variables, data);

    }

    slave->terminate();

    if (options_.failOnLargeFileSize) {
        const size_t size = data.size();
        if (size >= 1e6) {
            double mbSize = ((double) size) / 1e6;
            throw Rejection(string("Generated csv was larger than 1MB. Was: ") + std::to_string((mbSize)) + "MB");
        }
    }

    dumpOutput(data, options_.outputFolder.string());

}

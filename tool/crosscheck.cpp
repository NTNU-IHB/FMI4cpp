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

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <memory>

#include <stdlib.h>

#include <experimental/filesystem>
#include <boost/algorithm/string.hpp>

#include <fmi4cpp/common/logger.hpp>
#include <fmi4cpp/common/tools/os_util.hpp>
#include <fmi4cpp/common/driver/fmu_driver.hpp>

using namespace std;

using namespace fmi4cpp::fmi2;
using namespace fmi4cpp::driver;

namespace fs = std::experimental::filesystem;

namespace {

    void write(const fs::path &file, const string &data) {
        ofstream out(file, ofstream::out);
        out << data;
        out.flush();
        out.close();
    }

    string readFile(const fs::path &file) {
        ifstream stream(file.string());
        if (stream.is_open()) {
            return string((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
        } else {
            return {};
        }
    }

    string readLine(const fs::path &file) {
        ifstream stream(file.string());
        if (stream.is_open()) {
            string line;
            getline(stream, line);
            return line;
        } else {
            return {};
        }
    }

    vector<string> splitString(const string &txt, const char delimiter) {
        vector<string> lines;
        boost::split(lines, txt, [&delimiter](char c) { return c == delimiter; });
        return lines;
    }

    vector<std::string> parseVariables(string txt) {
        vector<string> vars;
        boost::split(vars, txt, [](char c) { return c == ','; });
        vars.erase(vars.begin()); //remove "time"
        for (auto &var : vars) {
            replace(var.begin(), var.end(), '\"', ' ');
            boost::trim(var);
        }
        return vars;
    }

    driver_options parseDefaults(string txt) {

        driver_options opt;
        auto lines = splitString(txt, '\n');
        for (const auto &line : lines) {

            if (line.length() == 0) {
                break;
            }

            auto split = splitString(line, ',');
            auto token = split[0];
            auto value = split[1];

            if (token == "StartTime") {
                opt.startTime = std::atof(value.c_str());
            } else if (token == "StopTime") {
                opt.stopTime = std::atof(value.c_str());
            } else if (token == "StepSize") {
                opt.stepSize = std::atof(value.c_str());
            }

        }

        return opt;
    }

}

namespace fmi4cpp::xc {

    class CrossChecker {

    public:

        void run(const fs::path &fmuDir, const fs::path &resultDir) {

            fmi4cpp::logger::info("Cross-checking FMU '{}'!", fmuDir.string());

            fs::create_directories(resultDir);

            writeReadme(resultDir);

            try {
                const auto fmuFile = (fmuDir / fmuDir.filename()).string() + ".fmu";
                const auto optFile = (fmuDir / fmuDir.filename()).string() + "_ref.opt";
                const auto refFile = (fmuDir / fmuDir.filename()).string() + "_ref.csv";
                const auto inFile = (fmuDir / fmuDir.filename()).string() + "_in.csv";

                auto hasInput = fs::exists(inFile);

                auto opt = parseDefaults(readFile(optFile));
                opt.outputFolder = resultDir;
                opt.failOnLargeFileSize = true;

                try {

                    if (opt.startTime >= opt.stopTime) {
                        throw Rejection("Invalid start and/or stop time (startTime >= stopTime).");
                    } else if (opt.stepSize == 0.0) {
                        throw Failure("Don't know how to handle variable step solver (stepsize=0.0).");
                    } else if (hasInput) {
                        throw Failure("Unable to handle input files yet.");
                    } else if (fmuDir.string().find("MapleSim") != std::string::npos) {
                        throw Rejection("asd");
                    }

                    opt.variables = parseVariables(readLine(refFile));

                    fmu_driver driver(fmuFile, opt);
                    driver.run();

                    pass(resultDir);
                    fmi4cpp::logger::info("Cross-checking FMU '{}' passed!", fmuDir.string());

                } catch (Rejection &ex) {
                    fmi4cpp::logger::warn("Cross-checking FMU '{}' rejected! {}", fmuDir.string(), ex.what());
                    reject(resultDir, ex.what());
                } catch (Failure &ex) {
                    fmi4cpp::logger::error("Cross-checking FMU '{}' failed! {}", fmuDir.string(), ex.what());
                    fail(resultDir, ex.what());
                }


            } catch (exception &ex) {
                fmi4cpp::logger::error("Cross-checking FMU '{}' failed. An unexpected program error occurred: {}",
                                       fmuDir.string(), ex.what());
                fail(resultDir, "An unexpected program error occurred");
            }

        }


    private:

        void fail(const fs::path &resultDir, const string &message) {
            write(resultDir / "failed", "Reason: " + message);
        }

        void reject(const fs::path &resultDir, const string &message) {
            write(resultDir / "rejected", "Reason: " + message);
        }

        void pass(const fs::path &resultDir) {
            write(resultDir / "passed", "");
        }

        void writeReadme(const fs::path &resultDir) {
            write(resultDir / "README.md",
                  "    The cross-check results have been generated with FMI4cpp's fmu_driver.\n"
                  "    To get more information download the 'fmu_driver' tool from https://github.com/SFI-Mechatronics/FMI4cpp/releases and run:\n"
                  "\n"
                  "    ```\n"
                  "    fmu_driver -h\n"
                  "    ```");
        }


    };

}


int main(int argc, char **argv) {

    if (! argv[1]) {
        cerr << "Path to XC directory missing!" << endl;
        return 1;
    }

    const string VERSION = "0.5.0";
    const string os = getOs();

    const fs::path xc_dir = fs::path(argv[1]);
    const fs::path csFmus = xc_dir / "fmus/2.0/cs" / os;
    const fs::path csResults = xc_dir / "results/2.0/cs" / os / "FMI4cpp" / VERSION;

    if (fs::exists(csResults)) {
        fs::remove_all(csResults);
    }

    fmi4cpp::xc::CrossChecker xc;
    for (const auto &vendor : fs::directory_iterator(csFmus)) {

        for (const auto &version : fs::directory_iterator(vendor)) {
            for (const auto &fmuDir : fs::directory_iterator(version)) {
                fs::path resultDir =
                        csResults / vendor.path().filename() / version.path().filename() / fmuDir.path().filename();
                xc.run(fmuDir, resultDir);
            }
        }

    }

    return 0;

}

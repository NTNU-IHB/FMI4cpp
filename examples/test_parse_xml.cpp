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

#include <iostream>
#include <fmicpp/fmi2/fmicpp.hpp>
#include <fmicpp/fmi2/import/FmiLibrary.hpp>

using namespace std;
using namespace fmicpp::fmi2::xml;
using namespace fmicpp::fmi2::import;

int main() {

    ModelDescription md;
    md.load("/home/laht/Downloads/modelDescription.xml");
    cout << "fmiVersion=" << md.fmiVersion <<endl;
    cout << "modelName=" << md.modelName <<endl;
    cout << "author=" << md.author <<endl;
    cout << "modelIdentifier=" << md.coSimulation->modelIdentifier << endl;
    cout << "Default stepSize=" << md.defaultExperiment->stepSize << endl;
    cout << "A sourcefile is named:" << md.coSimulation->sourceFiles->at(0).name << endl;
    auto& var = md.modelVariables->getByValueReference(1);
    cout << "A variable is named:" << var.name << ", with start=" << var.asRealVariable().start << endl;

    FmiLibrary lib("/home/laht/Downloads/ControlledTemperature.so");
    cout << "fmi2GetVersion=" << lib.getVersion() << endl;
    cout << "fmi2GetTypesPlatform=" << lib.getTypesPlatform() << endl;


    return 0;

};


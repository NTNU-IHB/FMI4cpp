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

#define BOOST_TEST_MODULE ControlledTemperature_Modeldescription_Test

#include <string>
#include <boost/test/unit_test.hpp>
#include <fmi4cpp/tools/os_util.hpp>
#include <fmi4cpp/fmi2/fmi4cpp.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

const string fmu_path = string(getenv("TEST_FMUs"))
                        + "/FMI_2.0/CoSimulation/" + getOs() +
                        "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";

BOOST_AUTO_TEST_CASE(ControlledTemperature_test1) {

    import::Fmu fmu(fmu_path);
    auto& md = fmu.getModelDescription();
    auto md_cs = md.asCoSimulationModelDescription();

    BOOST_CHECK_EQUAL("2.0", md.getFmiVersion());
    BOOST_CHECK_EQUAL("ControlledTemperature", md.getModelName());

    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md.getGuid());
    BOOST_CHECK_EQUAL("{06c2700b-b39c-4895-9151-304ddde28443}", md_cs.getGuid());
    BOOST_CHECK_EQUAL("20-sim", md.getGenerationTool());
    BOOST_CHECK_EQUAL("20-sim", md_cs.getGenerationTool());

    BOOST_CHECK_EQUAL(true, md.supportsCoSimulation());
    BOOST_CHECK_EQUAL(false, md.supportsModelExchange());

    BOOST_CHECK_EQUAL(120, md.getModelVariables().size());
    BOOST_CHECK_EQUAL(120, md_cs.getModelVariables().size());

    auto heatCapacity1 = md.getVariableByName("HeatCapacity1.T0").asRealVariable();
    BOOST_CHECK_EQUAL(1, heatCapacity1.getValueReference());
    BOOST_CHECK_EQUAL(false, heatCapacity1.getMin().has_value());
    BOOST_CHECK_EQUAL(false, heatCapacity1.getMax().has_value());
    BOOST_CHECK_EQUAL(true, heatCapacity1.getStart().has_value());
    BOOST_CHECK_EQUAL(298.0, *heatCapacity1.getStart());
    BOOST_CHECK_EQUAL("starting temperature", heatCapacity1.getDescription());
    BOOST_CHECK_EQUAL(false, heatCapacity1.getQuantity().has_value());

    auto thermalConductor = md.getVariableByValueReference(12);
    BOOST_CHECK_EQUAL("TemperatureSource.T", thermalConductor.getName());
    BOOST_CHECK(fmi2Variability::tunable == thermalConductor.getVariability());
    BOOST_CHECK(fmi2Causality::parameter == thermalConductor.getCausality());

    xml::SourceFiles sourceFiles = md_cs.getSourceFiles();
    BOOST_CHECK_EQUAL(10, sourceFiles.size());
    BOOST_CHECK_EQUAL("EulerAngles.c", sourceFiles[0].getName());

    auto modelStructureOutputs = md.getModelStructure().getOutputs();
    BOOST_CHECK_EQUAL(2, modelStructureOutputs.size());
    BOOST_CHECK_EQUAL(115, modelStructureOutputs[0].getIndex());
    BOOST_CHECK_EQUAL(116, modelStructureOutputs[1].getIndex());

    auto de = md.getDefaultExperiment();
    BOOST_CHECK(de.has_value());
    BOOST_CHECK_EQUAL(0.0, *de->getStartTime());
    BOOST_CHECK_EQUAL(20.0, *de->getStopTime());
    BOOST_CHECK_EQUAL(1E-4, *de->getStepSize());
    BOOST_CHECK_EQUAL(false, de->getTolerance().has_value());

    auto mv = md.getModelVariables();
    for (const xml::ScalarVariable &v : mv) {
        if (v.getCausality() == fmi2Causality::output) {
            v.asRealVariable().setStart(99);
        }
    }

    vector<xml::ScalarVariable> outputs;
    mv.getByCausality(fmi2Causality::output, outputs);
    for (const xml::ScalarVariable &v : outputs) {
        BOOST_CHECK_EQUAL(99, v.asRealVariable().getStart().value());
    }

}
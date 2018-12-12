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

#define BOOST_TEST_MODULE FmuExportCrossCompile_Modeldescription_Test

#include <vector>
#include <string>
#include <boost/test/unit_test.hpp>
#include <fmi4cpp/tools/os_util.hpp>
#include <fmi4cpp/fmi2/fmi4cpp.hpp>

using namespace std;
using namespace fmi4cpp::fmi2;

const string fmu_path = string(getenv("TEST_FMUs"))
                        + "/2.0/cs/" + getOs() +
                        "/OpenModelica/v1.11.0/FmuExportCrossCompile/FmuExportCrossCompile.fmu";

BOOST_AUTO_TEST_CASE(FmuExportCrossCompile_test1) {

    fmi2Fmu fmu(fmu_path);
    auto md = fmu.getModelDescription();

    BOOST_CHECK_EQUAL("structured", md->variableNamingConvention().value());

    vector<Unknown> derivatives = md->modelStructure()->derivatives();
    BOOST_CHECK_EQUAL(2, derivatives.size());

    BOOST_CHECK_EQUAL(3, derivatives[0].index());
    BOOST_CHECK_EQUAL(4, derivatives[1].index());

    BOOST_CHECK_EQUAL(true, derivatives[0].dependencies().has_value());
    BOOST_CHECK_EQUAL(2, derivatives[0].dependencies().value()[0]);
    BOOST_CHECK_EQUAL("dependent", derivatives[0].dependenciesKind().value());

}

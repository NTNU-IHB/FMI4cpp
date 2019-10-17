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

#define BOOST_TEST_MODULE FmuExportCrossCompile_test1

#include <fmi4cpp/fmi2/fmi2.hpp>

#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>

using namespace std;
using namespace fmi4cpp::fmi2;

const string fmu_path = "../resources/fmus/2.0/cs/OpenModelica/v1.11.0/"
                        "FmuExportCrossCompile/modelDescription.xml";

BOOST_AUTO_TEST_CASE(FmuExportCrossCompile_test1)
{

    auto md = parse_model_description(fmu_path);

    BOOST_CHECK_EQUAL("structured", md->variable_naming_convention.value());

    vector<unknown> derivatives = md->model_structure->derivatives;
    BOOST_CHECK_EQUAL(2, derivatives.size());

    BOOST_CHECK_EQUAL(3, derivatives[0].index);
    BOOST_CHECK_EQUAL(4, derivatives[1].index);

    BOOST_CHECK_EQUAL(true, derivatives[0].dependencies.is_initialized());
    BOOST_CHECK_EQUAL(2, derivatives[0].dependencies.value()[0]);
    BOOST_CHECK_EQUAL("dependent", derivatives[0].dependencies_kind.value()[0]);
}

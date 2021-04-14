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

#include <fmi4cpp/fmi2/cs_library.hpp>
#include <fmi4cpp/library_helper.hpp>

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

cs_library::cs_library(const std::string& modelIdentifier,
    const std::shared_ptr<fmu_resource>& resource)
    : fmi2_library(modelIdentifier, resource)
{

    fmi2SetRealInputDerivatives_ = load_function<fmi2SetRealInputDerivativesTYPE*>(handle_,
        "fmi2SetRealInputDerivatives");
    fmi2GetRealOutputDerivatives_ = load_function<fmi2GetRealOutputDerivativesTYPE*>(handle_,
        "fmi2GetRealOutputDerivatives");

    fmi2DoStep_ = load_function<fmi2DoStepTYPE*>(handle_, "fmi2DoStep");
    fmi2CancelStep_ = load_function<fmi2CancelStepTYPE*>(handle_, "fmi2CancelStep");

    fmi2GetStatus_ = load_function<fmi2GetStatusTYPE*>(handle_, "fmi2GetStatusTYPE");
    fmi2GetRealStatus_ = load_function<fmi2GetRealStatusTYPE*>(handle_, "fmi2GetRealStatusTYPE");
    fmi2GetIntegerStatus_ = load_function<fmi2GetIntegerStatusTYPE*>(handle_, "fmi2GetIntegerStatusTYPE");
    fmi2GetBooleanStatus_ = load_function<fmi2GetBooleanStatusTYPE*>(handle_, "fmi2GetBooleanStatusTYPE");
    fmi2GetStringStatus_ = load_function<fmi2GetStringStatusTYPE*>(handle_, "fmi2GetStringStatusTYPE");
}

bool cs_library::step(
    fmi2Component c,
    const fmi2Real currentCommunicationPoint,
    const fmi2Real communicationStepSize,
    const bool noSetFMUStatePriorToCurrentPoint)
{
    return update_status_and_return_true_if_ok(
        fmi2DoStep_(c, currentCommunicationPoint, communicationStepSize, noSetFMUStatePriorToCurrentPoint));
}

bool cs_library::cancel_step(fmi2Component c)
{
    return update_status_and_return_true_if_ok(fmi2CancelStep_(c));
}

bool cs_library::set_real_input_derivatives(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    const std::vector<fmi2Integer>& order,
    const std::vector<fmi2Real>& value)
{
    return fmi2SetRealInputDerivatives_(c, vr.data(), vr.size(), order.data(), value.data());
}

bool cs_library::get_real_output_derivatives(
    fmi2Component c,
    const std::vector<fmi2ValueReference>& vr,
    const std::vector<fmi2Integer>& order,
    std::vector<fmi2Real>& value)
{
    return update_status_and_return_true_if_ok(
        fmi2GetRealOutputDerivatives_(c, vr.data(), vr.size(), order.data(), value.data()));
}

bool cs_library::get_status(
    fmi2Component c,
    const fmi2StatusKind s,
    fmi2Status& value)
{
    return update_status_and_return_true_if_ok(
        fmi2GetStatus_(c, s, &value));
}

bool cs_library::get_real_status(
    fmi2Component c,
    const fmi2StatusKind s,
    fmi2Real& value)
{
    return update_status_and_return_true_if_ok(
        fmi2GetRealStatus_(c, s, &value));
}

bool cs_library::get_integer_status(
    fmi2Component c,
    const fmi2StatusKind s,
    fmi2Integer& value)
{
    return update_status_and_return_true_if_ok(
        fmi2GetIntegerStatus_(c, s, &value));
}

bool cs_library::get_boolean_status(
    fmi2Component c,
    const fmi2StatusKind s,
    fmi2Boolean& value)
{
    return update_status_and_return_true_if_ok(
        fmi2GetBooleanStatus_(c, s, &value));
}

bool cs_library::get_string_status(
    fmi2Component c,
    const fmi2StatusKind s,
    fmi2String& value)
{
    return update_status_and_return_true_if_ok(
        fmi2GetStringStatus_(c, s, &value));
}

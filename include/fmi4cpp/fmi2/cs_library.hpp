
#ifndef FMI4CPP_FMI2COSIMULATIONLIBRARY_HPP
#define FMI4CPP_FMI2COSIMULATIONLIBRARY_HPP

#include <fmi4cpp/fmi2/fmi2_library.hpp>

namespace fmi4cpp::fmi2
{

class cs_library : public fmi2_library
{

private:
    fmi2SetRealInputDerivativesTYPE* fmi2SetRealInputDerivatives_;
    fmi2GetRealOutputDerivativesTYPE* fmi2GetRealOutputDerivatives_;

    fmi2DoStepTYPE* fmi2DoStep_;
    fmi2CancelStepTYPE* fmi2CancelStep_;

    fmi2GetStatusTYPE* fmi2GetStatus_;
    fmi2GetRealStatusTYPE* fmi2GetRealStatus_;
    fmi2GetIntegerStatusTYPE* fmi2GetIntegerStatus_;
    fmi2GetBooleanStatusTYPE* fmi2GetBooleanStatus_;
    fmi2GetStringStatusTYPE* fmi2GetStringStatus_;

public:
    cs_library(
        const std::string& modelIdentifier,
        const std::shared_ptr<fmu_resource>& resource);

    bool step(fmi2Component c, fmi2Real currentCommunicationPoint,
        fmi2Real communicationStepSize, bool noSetFMUStatePriorToCurrentPoint);

    bool cancel_step(fmi2Component c);

    bool set_real_input_derivatives(fmi2Component c,
        const std::vector<fmi2ValueReference>& vr,
        const std::vector<fmi2Integer>& order,
        const std::vector<fmi2Real>& value);

    bool get_real_output_derivatives(fmi2Component c,
        const std::vector<fmi2ValueReference>& vr,
        const std::vector<fmi2Integer>& order,
        std::vector<fmi2Real>& value);

    bool get_status(fmi2Component c, fmi2StatusKind s, fmi2Status& value);
    bool get_real_status(fmi2Component c, fmi2StatusKind s, fmi2Real& value);
    bool get_integer_status(fmi2Component c, fmi2StatusKind s, fmi2Integer& value);
    bool get_boolean_status(fmi2Component c, fmi2StatusKind s, fmi2Boolean& value);
    bool get_string_status(fmi2Component c, fmi2StatusKind s, fmi2String& value);
};

} // namespace fmi4cpp::fmi2

#endif //FMI4CPP_FMI2COSIMULATIONLIBRARY_HPP

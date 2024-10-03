
#ifndef FMI4CPP_FMUVARIABLEACCESSOR_HPP
#define FMI4CPP_FMUVARIABLEACCESSOR_HPP

#include <fmi4cpp/types.hpp>

#include <vector>

namespace fmi4cpp
{

class fmu_reader
{

public:
    virtual bool read_integer(fmi4cppValueReference vr, fmi4cppInteger& ref) = 0;
    virtual bool read_integer(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppInteger>& ref) = 0;

    virtual bool read_real(fmi4cppValueReference vr, fmi4cppReal& ref) = 0;
    virtual bool read_real(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppReal>& ref) = 0;

    virtual bool read_string(fmi4cppValueReference vr, fmi4cppString& ref) = 0;
    virtual bool read_string(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppString>& ref) = 0;

    virtual bool read_boolean(fmi4cppValueReference vr, fmi4cppBoolean& ref) = 0;
    virtual bool read_boolean(const std::vector<fmi4cppValueReference>& vr, std::vector<fmi4cppBoolean>& ref) = 0;

    virtual ~fmu_reader() = default;
};

class fmu_writer
{

public:
    virtual bool write_integer(fmi4cppValueReference vr, fmi4cppInteger value) = 0;
    virtual bool write_integer(
        const std::vector<fmi4cppValueReference>& vr,
        const std::vector<fmi4cppInteger>& values) = 0;

    virtual bool write_real(fmi4cppValueReference vr, fmi4cppReal value) = 0;
    virtual bool write_real(
        const std::vector<fmi4cppValueReference>& vr,
        const std::vector<fmi4cppReal>& values) = 0;

    virtual bool write_string(fmi4cppValueReference vr, fmi4cppString value) = 0;
    virtual bool write_string(
        const std::vector<fmi4cppValueReference>& vr,
        const std::vector<fmi4cppString>& values) = 0;

    virtual bool write_boolean(fmi4cppValueReference vr, fmi4cppBoolean value) = 0;
    virtual bool write_boolean(
        const std::vector<fmi4cppValueReference>& vr,
        const std::vector<fmi4cppBoolean>& values) = 0;

    virtual ~fmu_writer() = default;
};

class fmu_variable_accessor : public fmu_reader, public fmu_writer
{
};

} // namespace fmi4cpp

#endif //FMI4CPP_FMUVARIABLEACCESSOR_HPP

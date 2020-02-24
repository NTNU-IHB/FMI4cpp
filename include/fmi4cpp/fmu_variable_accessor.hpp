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
};

class fmu_writer
{

public:
    virtual bool write_integer(fmi4cppValueReference vr, fmi4cppInteger value) = 0;
    virtual bool write_integer(const std::vector<fmi4cppValueReference>& vr,
        const std::vector<fmi4cppInteger>& values) = 0;

    virtual bool write_real(fmi4cppValueReference vr, fmi4cppReal value) = 0;
    virtual bool write_real(const std::vector<fmi4cppValueReference>& vr, const std::vector<fmi4cppReal>& values) = 0;

    virtual bool write_string(fmi4cppValueReference vr, fmi4cppString value) = 0;
    virtual bool write_string(const std::vector<fmi4cppValueReference>& vr,
        const std::vector<fmi4cppString>& values) = 0;

    virtual bool write_boolean(fmi4cppValueReference vr, fmi4cppBoolean value) = 0;
    virtual bool write_boolean(const std::vector<fmi4cppValueReference>& vr,
        const std::vector<fmi4cppBoolean>& values) = 0;
};

class fmu_variable_accessor : public fmu_reader, public fmu_writer
{
};

} // namespace fmi4cpp

#endif //FMI4CPP_FMUVARIABLEACCESSOR_HPP

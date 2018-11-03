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

#include <vector>
#include <string>

namespace fmi4cpp::fmi2 {

    class FmuReader {

    public:

        virtual bool readInteger(unsigned int vr, int &ref) = 0;
        virtual bool readInteger(const std::vector<unsigned int> &vr, std::vector<int> &ref) = 0;

        virtual bool readReal(unsigned int vr, double &ref) = 0;
        virtual bool readReal(const std::vector<unsigned int> &vr, std::vector<double> &ref) = 0;

        virtual bool readString(unsigned int vr, const char* &ref) = 0;
        virtual bool readString(const std::vector<unsigned int> &vr, std::vector<const char*> &ref) = 0;

        virtual bool readBoolean(unsigned int vr, int &ref) = 0;
        virtual bool readBoolean(const std::vector<unsigned int> &vr, std::vector<int > &ref) = 0;

    };

    class FmuWriter {

    public:

        virtual bool writeInteger(unsigned int vr, int value) = 0;
        virtual bool writeInteger(const std::vector<unsigned int> &vr, const std::vector<int > &values) = 0;

        virtual bool writeReal(unsigned int vr, double value) = 0;
        virtual bool writeReal(const std::vector<unsigned int> &vr, const std::vector<double > &values) = 0;

        virtual bool writeString(unsigned int vr, const char* value) = 0;
        virtual bool writeString(const std::vector<unsigned int> &vr, const std::vector<const char* > &values) = 0;

        virtual bool writeBoolean(unsigned int vr, int value) = 0;
        virtual bool writeBoolean(const std::vector<unsigned int> &vr, const std::vector<int > &values) = 0;
    };
    
    class FmuVariableAccessor: public FmuReader, public FmuWriter {};
    
}

#endif //FMI4CPP_FMUVARIABLEACCESSOR_HPP

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
#include <iostream>
#include <fstream>

#include <zip.h>
#include <fmicpp/os_util.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
using namespace std;

int main() {

    const string fmu_path = string(getenv("TEST_FMUs"))
                            + "/FMI_2.0/CoSimulation/" + getOs() +
                            "/20sim/4.6.4.8004/ControlledTemperature/ControlledTemperature.fmu";

    int* err;
    zip* za = zip_open(fmu_path.c_str(), 0, err);
    if (za == nullptr) {
        return *err;
    }

    auto tmp_path_ = fs::temp_directory_path() /= fs::path(fmu_path).stem();
    create_directories(tmp_path_);

    struct zip_file *zf;
    struct zip_stat sb;
    for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            printf("Name: [%s], ", sb.name);
            printf("Size: [%llu], \n", sb.size);

            if (sb.size == 0) {
                fs::create_directories(tmp_path_ / sb.name);
            } else {
                zf = zip_fopen_index(za, i, 0);
                char* contents = new char[sb.size];
                zip_fread(zf, contents, 0);
                if (!ofstream ((tmp_path_ / sb.name).c_str()).write(contents, sb.size)) {
                    cerr << "error" << endl;
                }
                zip_fclose(zf);
            }

        }
    }

    zip_close(za);
    remove_all(tmp_path_);

    return 0;
}
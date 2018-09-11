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

#ifndef FMICPP_UNZIPPER_HPP
#define FMICPP_UNZIPPER_HPP

#include <iostream>
#include <sstream>
#include <zip.h>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace {

    bool extractContents(std::string zip_file, std::string tmp_path) {

        int* err = 0;
        zip* za = zip_open(zip_file.c_str(), 0, err);
        if (za == nullptr) {
            return false;
        }

        struct zip_file *zf;
        struct zip_stat sb;

        const int bufferSize = 1000;
        char* contents = (char*) malloc(sizeof(char) * bufferSize);
        zip_int64_t sum, len;
        for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
            if (zip_stat_index(za, i, 0, &sb) == 0) {

                std::string newFile = tmp_path + "/" + sb.name;

                if (sb.size == 0) {
                    fs::create_directories(newFile);
                } else {
                    zf = zip_fopen_index(za, i, 0);

                    std::ofstream file;
                    file.open(newFile, std::ios::out | std::ios::binary);

                    sum = 0;
                    while (sum != sb.size) {
                        len = zip_fread(zf, contents, bufferSize);
                        file.write(contents, len);
                        sum += len;
                    }

                    file.flush();
                    file.close();

                    zip_fclose(zf);
                }

            }
        }
        delete contents;
        zip_close(za);

        return true;

    }

}

#endif //FMICPP_UNZIPPER_HPP

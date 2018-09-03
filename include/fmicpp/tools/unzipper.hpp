//
// Created by LarsIvar on 03.09.2018.
//

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

        int* err;
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

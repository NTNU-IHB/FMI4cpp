
#ifndef FMI4CPP_UNZIPPER_HPP
#define FMI4CPP_UNZIPPER_HPP

#include <fmi4cpp/fs_portability.hpp>

#include <zip.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace
{

bool unzip(const std::string& zip_file, const std::string& tmp_path)
{
    int* err = nullptr;
    zip* za = zip_open(zip_file.c_str(), 0, err);
    if (za == nullptr) {
        return false;
    }

    struct zip_file* zf;
    struct zip_stat sb{};

    const int bufferSize = 1000;
    char* contents = (char*)malloc(sizeof(char) * bufferSize);
    zip_int64_t sum;
    zip_int64_t len;
    for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {

            const std::string newFile = tmp_path + "/" + sb.name;

            if (sb.size == 0) {
                fmi4cpp::fs::create_directories(newFile);
            } else {
                const auto containingDirectory = fmi4cpp::fs::path{newFile}.parent_path();
                if (!fmi4cpp::fs::exists(containingDirectory) && !fmi4cpp::fs::create_directories(containingDirectory)) {
                    return false;
                }
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
    free(contents);
    zip_close(za);

    return true;
}

} // namespace

#endif //FMI4CPP_UNZIPPER_HPP

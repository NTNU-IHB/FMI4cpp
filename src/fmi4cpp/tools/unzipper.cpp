
#include "fmi4cpp/tools/unzipper.hpp"

#include <zip.h>

#include <fstream>
#include <string>
#include <vector>

bool fmi4cpp::unzip(const std::filesystem::path& zip_file, const std::filesystem::path& tmp_path)
{
    int* err = nullptr;
    zip* za = zip_open(absolute(zip_file).string().c_str(), 0, err);
    if (za == nullptr) {
        return false;
    }

    struct zip_file* zf;
    // clang-format off
    struct zip_stat sb{};
    // clang-format on

    constexpr int bufferSize = 1000;
    std::vector<char> contents(bufferSize);
    for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {

            const std::filesystem::path newFile = tmp_path / sb.name;

            if (sb.size == 0) {
                create_directories(newFile);
            } else {
                const auto containingDirectory = newFile.parent_path();
                if (!exists(containingDirectory) && !create_directories(containingDirectory)) {
                    return false;
                }
                zf = zip_fopen_index(za, i, 0);

                std::ofstream file;
                file.open(newFile, std::ios::out | std::ios::binary);

                zip_int64_t sum = 0;
                while (sum != sb.size) {
                    zip_int64_t len = zip_fread(zf, contents.data(), bufferSize);
                    file.write(contents.data(), len);
                    sum += len;
                }

                file.flush();
                file.close();

                zip_fclose(zf);
            }
        }
    }
    zip_close(za);

    return true;
}

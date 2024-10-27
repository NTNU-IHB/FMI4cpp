
#ifndef FMI4CPP_UNZIPPER_HPP
#define FMI4CPP_UNZIPPER_HPP

#include <filesystem>

namespace fmi4cpp
{

bool unzip(const std::filesystem::path& zip_file, const std::filesystem::path& tmp_path);

} // namespace fmi4cpp

#endif // FMI4CPP_UNZIPPER_HPP

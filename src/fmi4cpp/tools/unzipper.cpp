
#include "fmi4cpp/tools/unzipper.hpp"

#include "fmi4cpp/mlog.hpp"

#include <fstream>
#include <string>


bool fmi4cpp::unzip(const std::filesystem::path& zip_file, const std::filesystem::path& tmp_path)
{
    // Check if the zip file exists
    if (!exists(zip_file)) {
        MLOG_ERROR("Error: Zip file does not exist: " + std::filesystem::absolute(zip_file).string());
        return false;
    }

#ifdef _WIN32
    // Windows: Use the built-in unzip command via PowerShell
    std::string command = "tar -xf \"" + absolute(zip_file).string() + "\" -C \"" + absolute(tmp_path).string() + "\"";
#else
    // Linux: Use the unzip command
    std::string command = "unzip -o \"" + zip_file.string() + "\" -d \"" + tmp_path.string() + "\" > /dev/null 2>&1";
#endif

    // Execute the command
    int result = std::system(command.c_str());

    if (result != 0) {
        MLOG_ERROR("Error: Unzipping failed with code " + result);
        return false;
    }

    return true;
}

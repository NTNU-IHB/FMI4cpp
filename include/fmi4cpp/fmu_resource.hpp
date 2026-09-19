
#ifndef FMI4CPP_FMURESOURCE_HPP
#define FMI4CPP_FMURESOURCE_HPP

#include <filesystem>
#include <string>

namespace fmi4cpp
{

class fmu_resource
{

private:
    const std::filesystem::path path_;
    const bool removeOnDestruction_;

public:
    // Own the directory by default, as for an extraction made by fmi4cpp.
    explicit fmu_resource(std::filesystem::path path);
    // Borrowed directories remain under the caller's lifetime management.
    fmu_resource(std::filesystem::path path, bool removeOnDestruction);

    [[nodiscard]] std::string resource_path() const;

    [[nodiscard]] std::string model_description_path() const;

    [[nodiscard]] std::string absolute_library_path(const std::string& modelIdentifier) const;

    [[nodiscard]] std::string get_model_description_xml() const;

    ~fmu_resource();
};

} // namespace fmi4cpp

#endif //FMI4CPP_FMURESOURCE_HPP

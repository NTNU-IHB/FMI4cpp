
#ifndef FMI4CPP_FS_PORTABILITY_HPP
#define FMI4CPP_FS_PORTABILITY_HPP

#if __has_include(<filesystem>)
#    include <filesystem>
namespace fmi4cpp
{
namespace fs = std::filesystem;
}
#else
#    include <experimental/filesystem>
namespace fmi4cpp
{
namespace fs = std::experimental::filesystem;
}
#endif

#endif //FMI4CPP_FS_PORTABILITY_HPP

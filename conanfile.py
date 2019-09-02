
from conans import ConanFile, CMake


class FMI4cppConan(ConanFile):

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = (
        "boost/1.66.0@conan/stable",
        "libzip/1.5.2@bincrafters/stable",
        "bzip2/1.0.8@conan/stable"
    )

    default_options = {
        "boost:shared": True
    }

    def imports(self):
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.pdb", dst="bin", keep_path=False)

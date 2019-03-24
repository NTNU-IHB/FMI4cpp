
from conans import ConanFile, CMake


class FMI4cppConan(ConanFile):

    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = (
        "boost/1.66.0@conan/stable",
        "libzip/1.5.1@bincrafters/stable"
    )

    options = {"curl": [True, False]}
    default_options = {
        "curl": False,
        "boost:shared": True,
        "libzip:shared": True
    }

    def imports(self):
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.pdb", dst="bin", keep_path=False)

    def configure(self):
        if self.options.curl:
            self.options["libcurl"].shared=True

    def requirements(self):
        if self.options.curl:
            self.requires("OpenSSL/1.0.2o@conan/stable")
            self.requires("libcurl/7.61.1@bincrafters/stable")

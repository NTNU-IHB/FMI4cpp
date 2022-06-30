import os

from conans import ConanFile, CMake, tools
from os import path


class FMI4cppConan(ConanFile):
    name = "fmi4cpp"
    author = "NTNU Aalesund"
    exports = "version.txt"
    license = "MIT"
    scm = {
        "type": "git",
        "url": "auto",
        "revision": "auto"
    }
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = (
        "pugixml/1.12.1",
        "libzip/1.7.3",
        "zlib/1.2.12",
    )

    options = {
        "shared": [True, False]
    }

    default_options = {
        "shared": True
    }

    def set_version(self):
        self.version = tools.load(path.join(self.recipe_folder, "version.txt")).strip()

    def imports(self):
        self.copy("*.dll", dst="bin", src="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.pdb", dst="bin", keep_path=False)

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["FMI4CPP_USING_CONAN"] = "ON"
        cmake.definitions["BUILD_SHARED_LIBS"] = "ON" if self.options.shared else "OFF"
        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["fmi4cpp"]

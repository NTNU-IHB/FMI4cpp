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
    generators = "cmake", "virtualrunenv"
    requires = (
        "boost/1.71.0",
        "libzip/1.7.3"
    )

    options = {
        "shared": [True, False]
    }

    default_options = {
        "shared": True,
        "boost:shared": True
    }

    def set_version(self):
        self.version = tools.load(path.join(self.recipe_folder, "version.txt")).strip()

    def imports(self):
        binDir = os.path.join("output", str(self.settings.build_type).lower(), "bin")
        self.copy("*.dll", dst=binDir, src="lib", keep_path=False)
        self.copy("*.dll", dst=binDir, keep_path=False)
        self.copy("*.pdb", dst=binDir, keep_path=False)

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["FMI4CPP_USING_CONAN"] = "ON"
        cmake.definitions["BUILD_SHARED_LIBS"] = self.options.shared
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

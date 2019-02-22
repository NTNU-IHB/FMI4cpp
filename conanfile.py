
from conans import ConanFile, CMake


class FMI4cppConan(ConanFile):
    name = "FMI4cpp"
    version = "0.6.0"
    license = "MIT"
    author = "Lars Ivar Hatledal [larsivarhatledal@gmail.com]"
    url = "https://github.com/NTNU-IHB/FMI4cpp"
    description = "FMI 2.0 implementation written in modern C++."
    topics = ("FMI", "co-simulation", "model-exchange", "cpp17")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "include/*", "src/*", "cmake/*", "CMakeLists.txt"

    requires = (
        "boost/1.66.0@conan/stable",
        "libzip/1.5.1@bincrafters/stable",
        "spdlog/1.3.1@bincrafters/stable"
    )

    options = {"curl": [True, False]}
    default_options = (
        "curl=False",
        "boost:shared=True",
        "libzip:shared=True"
    )

    def configure(self):
        if self.options.curl:
            self.options["libcurl"].shared=True

    def requirements(self):
        if self.options.curl:
            self.requires("OpenSSL/1.0.2o@conan/stable")
            self.requires("libcurl/7.61.1@bincrafters/stable")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["FMI4CPP_WITH_ODEINT"] = "ON"
        cmake.definitions["FMI4CPP_USING_CONAN"] = "ON"
        cmake.definitions["FMI4CPP_BUILD_TOOL"] = "OFF"
        cmake.definitions["FMI4CPP_USING_VCPKG"] = "OFF"
        cmake.definitions["FMI4CPP_BUILD_TESTS"] = "OFF"
        cmake.definitions["FMI4CPP_BUILD_EXAMPLES"] = "OFF"
        cmake.configure()
        cmake.build()

    def package_info(self):
        self.cpp_info.libs = ["fmi4cpp"]


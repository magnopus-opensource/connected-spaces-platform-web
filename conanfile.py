from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout


class ConnectedSpacesPlatformWebConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    default_options = {
        "tinyspline/*:cxx": False,
        "msgpack-cxx/*:use_boost": False,
    }

    def requirements(self):
        self.requires("fmt/12.1.0")
        self.requires("rapidjson/cci.20250205")
        self.requires("glm/1.0.1")
        self.requires("msgpack-cxx/7.0.0")
        self.requires("asyncplusplus/1.2")
        self.requires("tinyspline/0.6.0")

    def build_requirements(self):
        self.tool_requires("ninja/1.11.1")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.user_presets_path = 'ConanPresets.json'
        tc.generate()


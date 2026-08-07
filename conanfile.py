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

        # CSP links OpenUSD, so the installed CSPConfig.cmake does
        # find_dependency(pxr) and the exported csp-lib target references USD's
        # archives. Both have to resolve here, at the final link.
        #
        # This is a prebuilt package created locally from a tree built by USD's
        # build_usd.py; see recipes/openusd/README.md in the CSP repository. If
        # Conan reports it as missing, it has not been created on this machine.
        self.requires("openusd/26.11")

    def build_requirements(self):
        self.tool_requires("ninja/1.11.1")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.user_presets_path = 'ConanPresets.json'

        # The openusd package deliberately does not expose itself through
        # CMakeDeps: CSP consumes USD's own pxrConfig.cmake, because USD's
        # imported targets carry the -Wl,--whole-archive wrapping that keeps its
        # static schema registrations alive. So we point CMake at the package
        # ourselves, matching what the CSP repository's conanfile.py does.
        #
        # pxrConfig.cmake sits at the root of a USD install prefix rather than
        # under lib/cmake, and it resolves TBB via find_dependency(TBB CONFIG),
        # so both variables are needed.
        usd_root = self.dependencies["openusd"].package_folder.replace("\\", "/")
        tc.cache_variables["pxr_DIR"] = usd_root
        tc.cache_variables["TBB_DIR"] = f"{usd_root}/lib/cmake/TBB"

        tc.generate()


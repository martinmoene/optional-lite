from conans import ConanFile, CMake

class optional_liteConan(ConanFile):
    name = "optional_lite"
    version = "3.1.1"
    license = "Boost Software License - Version 1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    description = "Expected objects for C++11 and later"
    author = "Karl Wallner <kwallner@mail.de>"
    url = 'git@github.com:kwallner/optional-lite.git'
    scm = { "type": "git", "url": "auto", "revision": "auto" }
    no_copy_source = True

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()
        cmake.test()
        
    def package_info(self):
        self.env_info.optional_lite_DIR = self.package_folder
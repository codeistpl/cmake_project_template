from conans import ConanFile, CMake

class BraceChecker(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = "gtest/1.8.1@bincrafters/stable" # package 1.10.0 have some issue witho conanfile ... find package dosn't work with build_type=Debug
   generators = "cmake", "cmake_paths"

   def imports(self):
      self.copy("*.dll", dst="bin", src="bin") # From bin to bin
      self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

   def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()

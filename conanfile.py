from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.env import VirtualRunEnv

class App(ConanFile):
  name = "plane"
  version = "0.0.1"
  settings = "os", "compiler", "build_type", "arch"

  default_options = {
    "llvm-openmp/*:shared": False,
  }


  def requirements(self):
    self.requires("catch2/3.15.2")
    self.requires("entt/3.13.2")
    self.requires("glew/2.2.0")
    self.requires("glm/1.0.1")
    self.requires("ms-gsl/4.0.0")
    self.requires("nlohmann_json/3.12.0")
    self.requires("openal-soft/1.24.3")
    self.requires("sdl/2.32.10")
    self.requires("thorvg/0.15.16")
    self.requires("tinygltf/2.9.7")


  def layout(self):
    cmake_layout(self)


  def generate(self):
    tc = CMakeToolchain(self)
    tc.user_presets_path = 'ConanPresets.json'
    tc.generate()
    deps = CMakeDeps(self)
    deps.generate()
    # Example `$ source ./build/Release/generators/conanrun.sh`
    ms = VirtualRunEnv(self)
    ms.generate()

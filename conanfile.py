# type: ignore
from conan import ConanFile

class App(ConanFile):
  name = "plane"
  build_policy = "missing"
  generators = "CMakeDeps", "CMakeToolchain", "VirtualBuildEnv"
  settings = "os", "compiler", "build_type", "arch"

  def requirements(self):
    self.requires("catch2/3.5.2"),
    self.requires("entt/3.12.2"),
    self.requires("glm/cci.20230113"),
    self.requires("ms-gsl/4.0.0"),
    self.requires("nlohmann_json/3.11.2"),
    self.requires("tinygltf/2.5.0"),
    if self.settings.os == "Linux":
      self.requires("glew/2.2.0")
      self.requires("sdl/2.26.5")

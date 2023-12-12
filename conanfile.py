from conan import ConanFile

class App(ConanFile):
  requires = (
    "entt/3.12.2",
    "glew/2.2.0",
    "glm/cci.20230113",
    "range-v3/0.12.0",
    "sdl/2.26.5",
    "tinygltf/2.5.0",
  )

  generators = "CMakeDeps", "CMakeToolchain", "VirtualBuildEnv"

  settings = "os", "compiler", "build_type", "arch"

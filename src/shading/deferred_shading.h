#pragma once
#include <memory>

#include "src/shading/g_buffer.h" // IWYU pragma: export

import pln.meshes;
import pln.shaders;

class DeferredShading
{
private:
  std::unique_ptr<GBuffer> _g_buffer;
  std::unique_ptr<pln::shaders::Shader> _geometry_pass;
  std::unique_ptr<pln::shaders::Shader> _light_pass;
  std::unique_ptr<pln::meshes::Mesh> _quad;

public:
  DeferredShading(std::unique_ptr<pln::shaders::Shader> geometry_pass,
                  std::unique_ptr<pln::shaders::Shader> light_pass,
                  std::unique_ptr<pln::meshes::Mesh> quad,
                  unsigned int width,
                  unsigned int height);
  DeferredShading(const DeferredShading&) = delete;
  DeferredShading& operator=(const DeferredShading&) = delete;
  DeferredShading(DeferredShading&&) = delete;
  DeferredShading& operator=(DeferredShading&&) = delete;
  ~DeferredShading() = default;

  void use_geometry_pass();
  void use_light_pass();
  pln::shaders::Shader& geometry_pass();
  pln::shaders::Shader& light_path();
  void draw_quad();
  GBuffer& g_buffer();
};

#pragma once
#include "src/mesh.h"
#include "src/shader.h"
#include "src/shading/g_buffer.h" // IWYU pragma: export

class DeferredShading
{
private:
  std::unique_ptr<GBuffer> _g_buffer;
  std::unique_ptr<Shader> _geometry_pass;
  std::unique_ptr<Shader> _light_pass;
  std::unique_ptr<Mesh> _quad;

public:
  DeferredShading(std::unique_ptr<Shader> geometry_pass,
                  std::unique_ptr<Shader> light_pass,
                  std::unique_ptr<Mesh> quad,
                  unsigned int width,
                  unsigned int height);
  DeferredShading(const DeferredShading&) = delete;
  DeferredShading& operator=(const DeferredShading&) = delete;
  DeferredShading(DeferredShading&&) = delete;
  DeferredShading& operator=(DeferredShading&&) = delete;
  ~DeferredShading() = default;

  void use_geometry_pass();
  void use_light_pass();
  Shader& geometry_pass();
  Shader& light_path();
  void draw_quad();
  GBuffer& g_buffer();
};

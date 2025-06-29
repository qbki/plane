module;
#include <glm/mat4x4.hpp>
#include <utility>
#include <memory>

export module pln.shading.deferred_shading;

import pln.mesh;
import pln.shaders;
import pln.shading.g_buffer;

namespace pln::shading {

export
class DeferredShading
{
private:
  std::unique_ptr<GBuffer> _g_buffer;
  std::unique_ptr<pln::shaders::Shader> _geometry_pass;
  std::unique_ptr<pln::shaders::Shader> _light_pass;
  std::unique_ptr<pln::mesh::Mesh> _quad;

public:
  DeferredShading(const DeferredShading&) = delete;
  DeferredShading& operator=(const DeferredShading&) = delete;
  DeferredShading(DeferredShading&&) = delete;
  DeferredShading& operator=(DeferredShading&&) = delete;
  ~DeferredShading() = default;

  DeferredShading(std::unique_ptr<pln::shaders::Shader> geometry_pass,
                  std::unique_ptr<pln::shaders::Shader> light_pass,
                  std::unique_ptr<pln::mesh::Mesh> quad,
                  unsigned int width,
                  unsigned int height)
    : _g_buffer(std::make_unique<GBuffer>())
    , _geometry_pass(std::move(geometry_pass))
    , _light_pass(std::move(light_pass))
    , _quad(std::move(quad))
  {
    _g_buffer->update(width, height);
  }


  void
  use_geometry_pass()
  {
    _geometry_pass->use();
  }


  void
  use_light_pass()
  {
    _light_pass->use();
    _g_buffer->use();
  }


  pln::shaders::Shader&
  geometry_pass()
  {
    return *_geometry_pass;
  }


  pln::shaders::Shader&
  light_path()
  {
    return *_light_pass;
  }


  void
  draw_quad()
  {
    pln::mesh::Mesh::DrawParams draw_params { .transforms { glm::mat4 {} } };
    _quad->draw(draw_params);
  }


  GBuffer&
  g_buffer()
  {
    return *_g_buffer;
  }
};

}

#include <glm/mat4x4.hpp>
#include <utility>

#include "deferred_shading.h"

DeferredShading::DeferredShading(std::unique_ptr<Shader> geometry_pass,
                                 std::unique_ptr<Shader> light_pass,
                                 std::unique_ptr<Mesh> quad,
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
DeferredShading::use_geometry_pass()
{
  _geometry_pass->use();
}

void
DeferredShading::use_light_pass()
{
  _light_pass->use();
  _g_buffer->use();
}

Shader&
DeferredShading::geometry_pass()
{
  return *_geometry_pass;
}

Shader&
DeferredShading::light_path()
{
  return *_light_pass;
}

void
DeferredShading::draw_quad()
{
  Mesh::DrawParams draw_params { .transforms { glm::mat4(1) } };
  _quad->draw(draw_params);
}

GBuffer&
DeferredShading::g_buffer()
{
  return *_g_buffer;
}

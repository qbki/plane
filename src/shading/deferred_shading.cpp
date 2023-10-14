#include "deferred_shading.h"


DeferredShading::DeferredShading(
  std::unique_ptr<Shader> geometry_pass,
  std::unique_ptr<Shader> light_pass,
  std::unique_ptr<Mesh> quad,
  unsigned int width,
  unsigned int height
) :
  _geometry_pass(std::move(geometry_pass)),
  _light_pass(std::move(light_pass)),
  _quad(std::move(quad)),
  _g_buffer(std::make_unique<GBuffer>())
{
  _g_buffer->update(width, height);
}


void DeferredShading::use_geometry_pass() {
  glBindFramebuffer(GL_FRAMEBUFFER, _g_buffer->g_buffer_handle());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _geometry_pass->use();
}


void DeferredShading::use_light_pass() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _light_pass->use();
  _g_buffer->use();
}


Shader& DeferredShading::geometry_pass() {
  return *_geometry_pass;
}


Shader& DeferredShading::light_path() {
  return *_light_pass;
}


void DeferredShading::draw_quad() {
  _quad->draw();
}


GBuffer& DeferredShading::g_buffer() {
  return *_g_buffer;
}

#include <GL/glew.h>
#include <SDL_pixels.h>
#include <SDL_surface.h>
#include <algorithm>
#include <format>
#include <string>
#include <unordered_map>

#include "src/components/common.h"
#include "src/components/textures.h"
#include "src/components/transform.h"
#include "src/consts.h"
#include "src/gui/core/font.h"
#include "src/gui/types.h"
#include "src/material.h"
#include "src/math/intersection.h"
#include "src/math/shapes.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/texture.h"
#include "src/utils/common.h"
#include "src/utils/ecs.h"

#include "render.h"

static const Material COMMON_MATERIAL(glm::vec3(0.05, 0.05, 0.05),
                                      glm::vec3(1.0, 1.0, 1.0),
                                      22);

struct TransformHolder
{
  Mesh* mesh = nullptr;
  const Textures* textures = nullptr;
  Mesh::DrawParams draw_params;
};

void
draw(std::unordered_map<Mesh*, TransformHolder>& transform_holders)
{
  for (auto& [_, holder] : transform_holders) {
    holder.textures->use();
    holder.mesh->draw(holder.draw_params);
  }
}

void
update_transform_mapping(
  std::unordered_map<Mesh*, TransformHolder>& transform_mapping,
  Mesh* mesh_pointer,
  const Textures* textures_pointer,
  glm::mat4& transform)
{
  if (transform_mapping.contains(mesh_pointer)) {
    auto& holder = transform_mapping[mesh_pointer];
    holder.draw_params.transforms.push_back(transform);
    holder.draw_params.texture_indices.push_back(
      textures_pointer->texture_type());
  } else {
    transform_mapping[mesh_pointer] = { .mesh = mesh_pointer,
                                        .textures = textures_pointer,
                                        .draw_params{
                                          .transforms{ transform },
                                          .texture_indices{
                                            textures_pointer->texture_type() },
                                        } };
  }
}

void
render_generic_objects(App& app, const Scene& scene)
{
  auto& deferred_shading = app.deferred_shading();
  auto& registry = scene.state().registry();
  const auto& camera = scene.state().camera();
  auto frustum = camera.frustum();
  auto inter_fb_handle = app.intermediate_fb().handle();
  auto g_buffer_handle = deferred_shading.g_buffer().g_buffer_handle();

  deferred_shading.use_geometry_pass();
  glBindFramebuffer(GL_FRAMEBUFFER, g_buffer_handle);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  auto& geometry_pass_shader = deferred_shading.geometry_pass();
  geometry_pass_shader.uniform("u_PV", camera.pv());

  std::unordered_map<Mesh*, TransformHolder> transform_mapping;

  glDisable(GL_BLEND);
  registry
    .view<const Transform,
          const MeshPointer,
          const Textures,
          const Opaque,
          const Available>()
    .each([&](const Transform& transform,
              const MeshPointer& mesh,
              const Textures& textures) {
      const auto transformed_collider =
        apply_transform_to_collider(transform, mesh->bounding_volume());
      if (!is_in_frustum(frustum, transformed_collider)) {
        return;
      }
      auto& matrix = transform.matrix();
      update_transform_mapping(
        transform_mapping, mesh.get(), &textures, matrix);
    });
  geometry_pass_shader.uniform("u_primary_texture", 0);
  geometry_pass_shader.uniform("u_secondary_texture", 1);
  draw(transform_mapping);
  transform_mapping.clear();

  auto screen_size = app.screen_size();

  glBindFramebuffer(GL_FRAMEBUFFER, inter_fb_handle);

  deferred_shading.use_light_pass();
  auto& light_pass_shader = deferred_shading.light_path();
  registry.view<Color, Direction, DirectionalLightKind>().each(
    [&](const Color& sun_color, const Direction& sun_direction) {
      light_pass_shader.uniform("u_camera_pos", camera.position());
      light_pass_shader.uniform("u_light.color", sun_color.value);
      light_pass_shader.uniform("u_light.direction", sun_direction.value);
      light_pass_shader.uniform("u_material.ambient",
                                COMMON_MATERIAL.ambient());
      light_pass_shader.uniform("u_material.specular",
                                COMMON_MATERIAL.specular());
      light_pass_shader.uniform("u_material.shininess",
                                COMMON_MATERIAL.shininess());
      light_pass_shader.uniform("u_position_texture", 0);
      light_pass_shader.uniform("u_normal_texture", 1);
      light_pass_shader.uniform("u_base_color_texture", 2);
    });

  int light_idx = 0;
  registry
    .view<const Transform,
          const Color,
          const PointLightParams,
          const PointLightKind>()
    .each([&](const Transform& transform,
              const Color& color,
              const PointLightParams& params) {
      auto u_color = std::format("u_point_lights[{}].color", light_idx);
      auto u_position = std::format("u_point_lights[{}].position", light_idx);
      auto u_constant = std::format("u_point_lights[{}].constant", light_idx);
      auto u_linear = std::format("u_point_lights[{}].linear", light_idx);
      auto u_quadratic = std::format("u_point_lights[{}].quadratic", light_idx);
      light_pass_shader.uniform(u_color, color.value);
      light_pass_shader.uniform(u_position, transform.translation());
      light_pass_shader.uniform(u_constant, params.constant);
      light_pass_shader.uniform(u_linear, params.linear);
      light_pass_shader.uniform(u_quadratic, params.quadratic);
      light_idx += 1;
    });
  light_pass_shader.uniform("u_point_lights_quantity", light_idx);

  deferred_shading.draw_quad();

  glBindFramebuffer(GL_READ_FRAMEBUFFER, g_buffer_handle);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, inter_fb_handle);
  glBlitFramebuffer(0,
                    0,
                    static_cast<GLint>(screen_size.width),
                    static_cast<GLint>(screen_size.height),
                    0,
                    0,
                    static_cast<GLint>(screen_size.width),
                    static_cast<GLint>(screen_size.height),
                    GL_DEPTH_BUFFER_BIT,
                    GL_NEAREST);
}

void
render_particles(App& app, const Scene& scene)
{
  auto& registry = scene.state().registry();
  const auto& camera = scene.state().camera();
  auto& particle_shader = app.particle_shader();
  particle_shader.use();
  particle_shader.uniform("u_PV", camera.pv());
  std::unordered_map<Mesh*, TransformHolder> transform_mapping;
  registry
    .view<const Transform,
          const MeshPointer,
          const Textures,
          const Available,
          const ParticleKind>()
    .each([&transform_mapping](const Transform& transform,
                               const MeshPointer& mesh,
                               const Textures& textures) {
      auto& matrix = transform.matrix();
      update_transform_mapping(
        transform_mapping, mesh.get(), &textures, matrix);
    });
  particle_shader.uniform("u_main_texture", 0);
  draw(transform_mapping);
}

void
render_ui(const Scene& scene)
{
  auto& registry = scene.state().registry();
  const auto& camera = scene.state().camera();
  auto& particle_shader = Services::app().particle_shader();
  auto inter_fb = Services::app().intermediate_fb().handle();
  particle_shader.use();
  particle_shader.uniform("u_PV", camera.pv());
  particle_shader.uniform("u_main_texture", 0);
  glBindFramebuffer(GL_FRAMEBUFFER, inter_fb);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  registry
    .view<Transform, UniqueTexturePtr, RectSize, Parent, Available, GUIKind>()
    .each([&registry](Transform& orig_transform,
                      UniqueTexturePtr& texture,
                      RectSize& rect,
                      Parent& parent) {
      Transform transform{};
      transform.translate({
        orig_transform.translation().x + static_cast<float>(rect.width) * HALF,
        orig_transform.translation().y + static_cast<float>(rect.height) * HALF,
        transform.translation().z,
      });
      transform.scale({ rect.width, rect.height, 1 });
      texture->use(0);

      glm::mat4 global_matrix =
        get_global_matrix(registry, parent) * transform.matrix();
      Services::gui_quad().draw({
        .transforms = std::vector<glm::mat4>{ global_matrix },
        .texture_indices =
          std::vector<TextureType::Type>{ TextureType::Type::PRIMARY },
      });
    });
}

void
update_font_texture(const Scene& scene)
{
  auto& registry = scene.state().registry();
  registry
    .view<GUI::FontPtr,
          UniqueTexturePtr,
          RectSize,
          Text,
          Core::Color,
          IsDirty,
          GUIKind>()
    .each([](GUI::FontPtr& font,
             UniqueTexturePtr& texture,
             RectSize& rect,
             Text& _text,
             Core::Color& color,
             IsDirty& _is_dirty) {
      const auto color_size = 4;
      auto& text = _text.value;
      auto& is_dirty = _is_dirty.value;
      if (!is_dirty) {
        return;
      }
      is_dirty = false;
      auto update = [&](const std::vector<unsigned char>& data) {
        if (texture->width() == rect.width &&
            texture->height() == rect.height) {
          texture->data(data);
        } else {
          texture->data(data, rect.width, rect.height);
        }
      };
      if (text.empty()) {
        rect.width = 1;
        rect.height = font->size();
        std::vector<unsigned char> result(rect.height, 0);
        update(result);
        return;
      }
      auto font_surface = font->draw(text, color);
      rect.width = font_surface->w;
      rect.height = font_surface->h;
      auto result = get_pixels(rect.width, rect.height);
      SDL_ConvertPixels(rect.width,
                        rect.height,
                        font_surface->format->format,
                        font_surface->pixels,
                        font_surface->pitch,
                        SDL_PIXELFORMAT_ABGR8888,
                        result.data(),
                        rect.width * color_size);
      auto length = rect.height / 2;
      auto begin = result.begin();
      auto end = result.end();
      auto width = rect.width * color_size;
      for (int y = 0; y < length; y++) {
        auto src_start = begin + y * width;
        auto src_end = src_start + width;
        auto dst_start = end - width - width * y;
        std::swap_ranges(src_start, src_end, dst_start);
      }
      update(result);
    });
}

void
render_system(App& app)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  auto& inter_fb = app.intermediate_fb();
  auto& inter_shader = app.intermediate_shader();
  for (const auto& scene : app.scenes()) {
    glBindFramebuffer(GL_FRAMEBUFFER, inter_fb.handle());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (scene->is_deferred()) {
      render_generic_objects(app, *scene);
      render_particles(app, *scene);
    } else {
      update_font_texture(*scene);
      render_ui(*scene);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    inter_fb.use();
    inter_shader.use();
    inter_shader.uniform("u_gamma", GAMMA);
    inter_shader.uniform("u_main_texture", 0);
    app.deferred_shading().draw_quad();
  }
}

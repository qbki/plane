#include <sstream>

#include "../components.h"
#include "../consts.h"
#include "render.h"

static const Material COMMON_MATERIAL(glm::vec3(0.01, 0.01, 0.01),
                                      glm::vec3(1.0, 1.0, 1.0),
                                      22);

void
render_system(App::Meta& meta)
{
  auto& registry = meta.app->game_state->registry();
  const auto& camera = *meta.app->game_state->camera();
  auto& deferred_shading = *meta.app->deferred_shading;
  auto& particle_shader = *meta.app->particle_shader;
  auto& screen_size = *meta.app->screen_size;

  glDisable(GL_BLEND);
  deferred_shading.use_geometry_pass();
  auto& geometry_pass_shader = deferred_shading.geometry_pass();
  geometry_pass_shader.uniform("u_PV", camera.pv());
  geometry_pass_shader.uniform("u_elapsed_seconds", meta.time);

  registry
    .view<const Position,
          const Rotation,
          const MeshPointer,
          const Textures,
          const Opaque,
          const Available>()
    .each([&geometry_pass_shader](const Position& position,
                                  const Rotation& rotation,
                                  const MeshPointer& mesh,
                                  const Textures& textures) {
      auto rotation_matrix =
        glm::rotate(glm::mat4(1.0), rotation.value.z, { 0.0, 0.0, 1.0 });
      auto transform_matrix = glm::translate(glm::mat4(1.0), position.value);
      auto transform = transform_matrix * rotation_matrix;
      geometry_pass_shader.uniform("u_M", transform);
      geometry_pass_shader.uniform(
        "u_normal_matrix", glm::transpose(glm::inverse(glm::mat3(transform))));
      textures.texture().use(0);
      geometry_pass_shader.uniform("main_texture", 0);
      mesh->draw();
    });

  auto [_sun_id, sun_color, sun_direction] =
    *registry.view<Color, Direction, DirectionalLightKind>().each().begin();

  deferred_shading.use_light_pass();
  auto& light_pass_shader = deferred_shading.light_path();
  light_pass_shader.uniform("u_camera_pos", camera.position());
  light_pass_shader.uniform("u_light.color", sun_color.value);
  light_pass_shader.uniform("u_light.direction", sun_direction.value);
  light_pass_shader.uniform("u_material.ambient", COMMON_MATERIAL.ambient());
  light_pass_shader.uniform("u_material.specular", COMMON_MATERIAL.specular());
  light_pass_shader.uniform("u_material.shininess",
                            COMMON_MATERIAL.shininess());
  light_pass_shader.uniform("u_gamma_in", GAMMA);
  light_pass_shader.uniform("u_gamma_out", GAMMA);
  light_pass_shader.uniform("u_position_texture", 0);
  light_pass_shader.uniform("u_normal_texture", 1);
  light_pass_shader.uniform("u_base_color_texture", 2);

  int lights_quantity = 0;
  registry
    .view<const Position,
          const Color,
          const PointLightParams,
          const PointLightKind>()
    .each([&](const Position& position,
              const Color& color,
              const PointLightParams& params) {
      std::stringstream prefix;
      prefix << "u_point_lights[" << lights_quantity << "].";
      light_pass_shader.uniform(prefix.str() + "color", color.value);
      light_pass_shader.uniform(prefix.str() + "position", position.value);
      light_pass_shader.uniform(prefix.str() + "constant", params.constant);
      light_pass_shader.uniform(prefix.str() + "linear", params.linear);
      light_pass_shader.uniform(prefix.str() + "quadratic", params.quadratic);
      lights_quantity += 1;
    });
  light_pass_shader.uniform("u_point_lights_quantity", lights_quantity);
  deferred_shading.draw_quad();

  glBindFramebuffer(GL_READ_FRAMEBUFFER,
                    deferred_shading.g_buffer().g_buffer_handle());
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  particle_shader.use();
  particle_shader.uniform("u_gamma_in", GAMMA);
  particle_shader.uniform("u_gamma_out", GAMMA);
  particle_shader.uniform("u_PV", camera.pv());

  registry
    .view<const Position,
          const Rotation,
          const MeshPointer,
          const Textures,
          const Available,
          const ParticleKind>(entt::exclude<Opaque>)
    .each([&particle_shader](const Position& position,
                             const Rotation& rotation,
                             const MeshPointer& mesh,
                             const Textures& textures) {
      auto transform = make_transform_matrix(position.value, rotation.value);
      particle_shader.uniform("u_M", transform);
      particle_shader.uniform(
        "u_normal_matrix", glm::transpose(glm::inverse(glm::mat3(transform))));
      textures.texture().use(0);
      particle_shader.uniform("u_main_texture", 0);
      mesh->draw();
    });
}

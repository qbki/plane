#include <sstream>

#include "../consts.h"
#include "render.h"


GameState::Handler make_render_system(
  DeferredShading& deferred_shading,
  Shader& particle_shader,
  Material& common_material,
  DirectionalLight& sun,
  std::vector<PointLight>& lights,
  int& screen_width,
  int& screen_height
) {
  return [&](GameState::Meta& meta) {
    auto& registry = meta.state.registry();
    auto& camera = meta.camera;

    glDisable(GL_BLEND);
    deferred_shading.use_geometry_pass();
    auto& geometry_pass_shader = deferred_shading.geometry_pass();
    geometry_pass_shader.uniform("u_PV", camera.pv());
    geometry_pass_shader.uniform("u_elapsed_seconds", meta.time);

    registry.view<
      const Position,
      const Rotation,
      const MeshPointer,
      const TexturePointer,
      const Opaque,
      const Available
    >().each([&geometry_pass_shader](
      const Position& position,
      const Rotation& rotation,
      const MeshPointer& mesh,
      const TexturePointer& texture
    ) {
      auto rotation_matrix = glm::rotate(glm::mat4(1.0), rotation.value.z, {0.0, 0.0, 1.0});
      auto transform_matrix = glm::translate(glm::mat4(1.0), position.value);
      auto transform = transform_matrix * rotation_matrix;
      geometry_pass_shader.uniform("u_M", transform);
      geometry_pass_shader.uniform(
        "u_normal_matrix",
        glm::transpose(glm::inverse(glm::mat3(transform))));
      texture->use(0);
      geometry_pass_shader.uniform("main_texture", 0);
      mesh->draw();
    });

    deferred_shading.use_light_pass();
    auto& light_pass_shader = deferred_shading.light_path();
    light_pass_shader.uniform("u_camera_pos", camera.position());
    light_pass_shader.uniform("u_light.color", sun.color());
    light_pass_shader.uniform("u_light.direction", sun.direction());
    light_pass_shader.uniform("u_material.ambient", common_material.ambient());
    light_pass_shader.uniform("u_material.specular", common_material.specular());
    light_pass_shader.uniform("u_material.shininess", common_material.shininess());
    light_pass_shader.uniform("u_gamma_in", GAMMA);
    light_pass_shader.uniform("u_gamma_out", GAMMA);
    light_pass_shader.uniform("u_position_texture", 0);
    light_pass_shader.uniform("u_normal_texture", 1);
    light_pass_shader.uniform("u_base_color_texture", 2);

    int lights_quantity = lights.size();
    light_pass_shader.uniform("u_point_lights_quantity", lights_quantity);
    for (auto i = 0; i < lights_quantity; i++) {
      auto& light = lights[i];
      std::stringstream prefix;
      prefix << "u_point_lights[" << i << "].";
      light_pass_shader.uniform(prefix.str() + "color", light.color());
      light_pass_shader.uniform(prefix.str() + "position", light.position());
      light_pass_shader.uniform(prefix.str() + "constant", light.constant());
      light_pass_shader.uniform(prefix.str() + "linear", light.linear());
      light_pass_shader.uniform(prefix.str() + "quadratic", light.quadratic());
    }
    deferred_shading.draw_quad();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, deferred_shading.g_buffer().g_buffer_handle());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(
      0, 0,
      screen_width, screen_height,
      0, 0,
      screen_width, screen_height,
      GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    particle_shader.use();
    particle_shader.uniform("u_gamma_in", GAMMA);
    particle_shader.uniform("u_gamma_out", GAMMA);
    particle_shader.uniform("u_PV", camera.pv());
    geometry_pass_shader.uniform("u_elapsed_seconds", meta.time);

    registry.view<
      const Position,
      const Rotation,
      const MeshPointer,
      const TexturePointer,
      const Available,
      const ParticleKind
    >(entt::exclude<Opaque>).each([&particle_shader](
      const Position& position,
      const Rotation& rotation,
      const MeshPointer& mesh,
      const TexturePointer& texture
    ) {
      auto transform = make_transform_matrix(position.value, rotation.value);
      particle_shader.uniform("u_M", transform);
      particle_shader.uniform(
        "u_normal_matrix",
        glm::transpose(glm::inverse(glm::mat3(transform))));
      texture->use(0);
      particle_shader.uniform("main_texture", 0);
      mesh->draw();
    });
  };
}

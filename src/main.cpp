#define GLM_ENABLE_EXPERIMENTAL
#include <vector>
#include <memory>
#include <sstream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

#include "camera.h"
#include "consts.h"
#include "control.h"
#include "game_state/index.h"
#include "lights/index.h"
#include "models/index.h"
#include "sdl_init.h"
#include "shading/index.h"
#include "utils.h"


const int DEFAULT_SCREEN_WIDTH = 800;
const int DEFAULT_SCREEN_HEIGHT = 600;
const float ANIMATED_OBJECTS_HEIGHT = 2.0;


int main() {
  int screen_width = DEFAULT_SCREEN_WIDTH;
  int screen_height = DEFAULT_SCREEN_HEIGHT;
  auto window = init_window(screen_width, screen_height);
  auto context = init_context(window.get());
  Control control;

  auto factory = std::make_unique<ModelFactory>();
  auto scene = std::vector<std::shared_ptr<Graphic>>();
  auto game_state = std::make_unique<GameState>();
  auto common_material = std::make_unique<Material>(
    glm::vec3(0.01, 0.01, 0.01),
    glm::vec3(1.0, 1.0, 1.0),
    22
  );

  auto particle_emitter = std::make_unique<ParticleEmitter>([&]() {
    return factory->make_particle();
  });
  game_state->particle_emitter(move(particle_emitter));

  auto geometry_pass_shader = std::make_unique<Shader>();
  auto geometry_pass_vertex_shader = load_text("./shaders/main_v.glsl");
  auto geometry_pass_fragment_shader = load_text("./shaders/deferred_geometry_pass_f.glsl");
  geometry_pass_shader->compile(geometry_pass_vertex_shader, geometry_pass_fragment_shader);

  auto light_pass_shader = std::make_unique<Shader>();
  auto light_pass_vertex_shader = load_text("./shaders/deferred_light_pass_v.glsl");
  auto light_pass_fragment_shader = load_text("./shaders/deferred_light_pass_f.glsl");
  light_pass_shader->compile(light_pass_vertex_shader, light_pass_fragment_shader);

  auto particle_shader = std::make_unique<Shader>();
  auto particle_vertex_shader = load_text("./shaders/main_v.glsl");
  auto particle_fragment_shader = load_text("./shaders/particle_f.glsl");
  particle_shader->compile(particle_vertex_shader, particle_fragment_shader);

  auto sun = std::make_unique<DirectionalLight>(
    glm::vec3(1.0, 1.0, 1.0),
    glm::vec3(-0.8, -0.8, -1.0)
  );

  std::vector<PointLight> lights {
    {{1, 0, 0}, {-5,  5, 8}},
    {{0, 1, 0}, {-5, -5, 8}},
    {{0, 0, 1}, { 5, -5, 8}},
    {{1, 0, 1}, {  0, -5, 8}},
  };

  for (auto& light : lights) {
    light.constant(1.0);
    light.linear(0.045);
    light.quadratic(0.0075);
  }

  game_state->camera(std::make_shared<Camera>(
    glm::vec3(0.0, -4.0, 15.0),
    static_cast<float>(screen_width) / screen_height
  ));

  {
    auto player = factory->make_player();
    player->position({0.0, -5.0, ANIMATED_OBJECTS_HEIGHT});
    game_state->player(player);
    scene.push_back(player);
  }

  {
    GameState::Entities enemies;
    for (int x = -10; x < 10; x++) {
      for (int y = 0; y < 30; y++) {
        auto model (factory->make_enemy());
        model->position({x, y, ANIMATED_OBJECTS_HEIGHT});
        enemies.push_back(model);
      }
    }
    game_state->add_enemies(enemies);
    std::copy(begin(enemies), end(enemies), std::back_inserter(scene));
  }

  {
    GameState::Projectiles projectiles;
    GameState::Entities graphics;
    {
      for (int x = 0; x < 100; x++) {
        auto projectile = factory->make_projectile();
        projectile.distance(8);
        projectiles.push_back(projectile);
        graphics.push_back(projectile.model());
      }
    }
    game_state->add_projectiles(projectiles);
    std::copy(begin(graphics), end(graphics), std::back_inserter(scene));
  }

  {
    GameState::Entities decorations;
    for (int y = -10; y < 10; y++) {
      for (int x = -15; x <= 15; x++) {
        auto model = rand() % 2 == 0
          ? factory->make_water_block()
          : factory->make_ground_block();
        model->position({x, y, 0.0});
        decorations.push_back(model);
      }
    }
    game_state->add_decoration(decorations);
    std::copy(begin(decorations), end(decorations), std::back_inserter(scene));
  }

  game_state->add_handler(make_cursor_handler(screen_width, screen_height));
  game_state->add_handler(handle_bullets);
  game_state->add_handler(handle_enemies_hunting);
  game_state->add_handler(handle_enemy_rotation);
  game_state->add_handler(handle_enemy_sinking);
  game_state->add_handler(handle_particles);
  game_state->add_handler(move_camera);
  game_state->add_handler(move_player);
  game_state->add_handler(rotate_player);
  game_state->add_handler(shoot_by_player);

  auto is_running = true;
  auto last_time_point = SDL_GetTicks64();
  SDL_Event event;

  auto deferred_shading = std::make_unique<DeferredShading>(
    std::move(geometry_pass_shader),
    std::move(light_pass_shader),
    Mesh::quad(0.99999),
    screen_width,
    screen_height
  );

  while (is_running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT: {
          is_running = false;
          break;
        }
        case SDL_KEYDOWN: {
          if (event.key.keysym.sym == SDLK_q) {
            is_running = false;
          }
          break;
        }
      }
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          screen_width = event.window.data1;
          screen_height = event.window.data2;
          resize_window(event.window, *game_state->camera());
          deferred_shading->g_buffer().update(screen_width, screen_height);
          break;
        }
      }
      control.update(event);
    }

    auto camera = game_state->camera();
    auto now = SDL_GetTicks64();
    auto seconds_since_last_frame = (now - last_time_point) * 0.001;
    last_time_point = now;

    game_state->update(control, seconds_since_last_frame);

    glDisable(GL_BLEND);
    deferred_shading->use_geometry_pass();
    auto& geometry_pass_shader = deferred_shading->geometry_pass();
    geometry_pass_shader.uniform("u_PV", camera->pv());
    geometry_pass_shader.uniform("u_elapsed_seconds", static_cast<float>(now * 0.001));
    for (auto& _model : scene) {
      auto model = dynamic_cast<Model*>(_model.get());
      if (model != nullptr) {
        geometry_pass_shader.uniform("u_M", model->transform());
        geometry_pass_shader.uniform(
          "u_normal_matrix",
          glm::transpose(glm::inverse(glm::mat3(model->transform()))));
        auto texture = model->texture();
        texture->use(0);
        geometry_pass_shader.uniform("main_texture", 0);
      }
      model->draw();
    }

    deferred_shading->use_light_pass();
    auto& light_pass_shader = deferred_shading->light_path();
    light_pass_shader.uniform("u_camera_pos", camera->position());
    light_pass_shader.uniform("u_light.color", sun->color());
    light_pass_shader.uniform("u_light.direction", sun->direction());
    light_pass_shader.uniform("u_material.ambient", common_material->ambient());
    light_pass_shader.uniform("u_material.specular", common_material->specular());
    light_pass_shader.uniform("u_material.shininess", common_material->shininess());
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
    deferred_shading->draw_quad();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, deferred_shading->g_buffer().g_buffer_handle());
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
    particle_shader->use();
    particle_shader->uniform("u_gamma_in", GAMMA);
    particle_shader->uniform("u_gamma_out", GAMMA);
    particle_shader->uniform("u_PV", camera->pv());
    particle_shader->uniform("u_elapsed_seconds", static_cast<float>(now * 0.001));
    for (auto& particle : game_state->particle_emitter().particles()) {
      auto& model = particle.model;
      particle_shader->uniform("u_M", model->transform());
      particle_shader->uniform(
        "u_normal_matrix",
        glm::transpose(glm::inverse(glm::mat3(model->transform()))));
      model->texture()->use(0);
      particle_shader->uniform("main_texture", 0);
      model->draw();
    }

    SDL_GL_SwapWindow(window.get());
  }
}

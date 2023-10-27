#pragma once
#include <functional>

#include "../game_state/index.h"
#include "../shading/index.h"
#include "../lights/index.h"


GameState::Handler make_render_system(
  DeferredShading& deferred_shading,
  Shader& particle_shader,
  Material& common_material,
  DirectionalLight& sun,
  std::vector<PointLight>& lights,
  int& screen_width,
  int& screen_height
);

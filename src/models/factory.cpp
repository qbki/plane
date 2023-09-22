#include <memory>

#include "factory.h"
#include "model.h"


ModelFactory::ModelFactory() : cache(new Cache()) {};


std::shared_ptr<Graphic> ModelFactory::make_player() {
  return cache->load(
    "./models/plane.glb",
    "./shaders/main_v.glsl",
    "./shaders/main_f.glsl"
  );
}


std::shared_ptr<Graphic> ModelFactory::make_enemy() {
  return cache->load(
    "./models/saucer.glb",
    "./shaders/main_v.glsl",
    "./shaders/main_f.glsl"
  );
}


std::shared_ptr<Graphic> ModelFactory::make_bullet() {
  auto model (cache->load(
    "./models/bullet.glb",
    "./shaders/main_v.glsl",
    "./shaders/main_f.glsl"
  ));
  model->is_active(false);
  return model;
}


std::shared_ptr<Graphic> ModelFactory::make_water_block() {
  return cache->load(
    "./models/water-surface.glb",
    "./shaders/water_v.glsl",
    "./shaders/main_f.glsl"
  );
}


std::shared_ptr<Graphic> ModelFactory::make_ground_block() {
  return cache->load(
    "./models/center-block.glb",
    "./shaders/main_v.glsl",
    "./shaders/main_f.glsl"
  );
}

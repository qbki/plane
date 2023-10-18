#include <memory>

#include "factory.h"
#include "projectile.h"


ModelFactory::ModelFactory() : cache(new Cache()) {};


std::shared_ptr<Graphic> ModelFactory::make_player() {
  return cache->load("./models/plane.glb");
}


std::shared_ptr<Graphic> ModelFactory::make_enemy() {
  return cache->load("./models/saucer.glb");
}


Projectile ModelFactory::make_projectile() {
  auto model = cache->load("./models/bullet.glb");
  model->is_active(false);
  return Projectile(model);
}


std::shared_ptr<Graphic> ModelFactory::make_water_block() {
  return cache->load("./models/water-surface.glb");
}


std::shared_ptr<Graphic> ModelFactory::make_ground_block() {
  return cache->load("./models/center-block.glb");
}

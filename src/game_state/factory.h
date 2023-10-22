#pragma once
#include <memory>

#include "../models/index.h"
#include "projectile.h"

class ModelFactory {
private:
  std::unique_ptr<Cache> cache;

public:
  ModelFactory();
  std::shared_ptr<Model> make_player();
  std::shared_ptr<Model> make_enemy();
  std::shared_ptr<Model> make_water_block();
  std::shared_ptr<Model> make_ground_block();
  std::shared_ptr<Model> make_particle();
  Projectile make_projectile();
};

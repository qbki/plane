#pragma once
#include <memory>

#include "graphic.h"

class Cache;
class ModelFactory {
private:
  std::unique_ptr<Cache> cache;

public:
  ModelFactory();
  std::shared_ptr<Graphic> make_player();
  std::shared_ptr<Graphic> make_enemy();
  std::shared_ptr<Graphic> make_bullet();
  std::shared_ptr<Graphic> make_water_block();
  std::shared_ptr<Graphic> make_ground_block();
};

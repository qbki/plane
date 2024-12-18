#pragma once
#include "src/scene/scene.h"

void
player_rotation_system(const Scene& scene);

void
player_shooting_system(const Scene& scene);

void
player_moving_system(const Scene& scene);

void
player_enemy_pointers(const Scene& scene);

class LoseSystem
{
private:
  bool is_fired = false;

public:
  void operator()(const Scene& scene);
};

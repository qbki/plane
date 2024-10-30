#pragma once
#include "src/scene/scene.h"

void
acceleration_system(const Scene& scene);

void
velocity_system(const Scene& scene);

void
velocity_gravity_system(const Scene& scene);

void
damping_system(const Scene& scene);

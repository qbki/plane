#pragma once
#include <filesystem>

#include "src/math/shapes.h"

const float GAMMA = 2.4;
const float GRAVITY = 9.80665; // m/(s^2)
const float FARTHEST_NDS_Z_COORD = 0.99999;

const int DEFAULT_SCREEN_WIDTH = 800;
const int DEFAULT_SCREEN_HEIGHT = 600;

const int DEFAULT_AUDIO_CHUNKSIZE = 256;
const int DEFAULT_MAX_CHANNELS = 100;

const float HALF = 0.5;

const std::filesystem::path SHADERS_DIR("shaders");
const std::filesystem::path DEFAULT_ASSETS_DIR("assets");
const std::filesystem::path SAVES_DIR("saves");
const std::filesystem::path SETTINGS_FILE(SAVES_DIR / "settings.json");
const std::filesystem::path SAVE_DATA_FILE(SAVES_DIR / "save.json");

const Shape::Sphere DEFAULT_COLLIDER { .radius = 0.5 };

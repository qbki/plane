module;
#include <filesystem>

#include "src/math/shapes.h"

export module pln.consts;

namespace pln::consts {

export const float GAMMA = 2.4;
export const float GRAVITY = 9.80665; // m/(s^2)
export const float FARTHEST_NDS_Z_COORD = 0.99999;

export const int DEFAULT_SCREEN_WIDTH = 800;
export const int DEFAULT_SCREEN_HEIGHT = 600;

export const int DEFAULT_AUDIO_CHUNKSIZE = 256;
export const int DEFAULT_MAX_CHANNELS = 100;

export const float HALF = 0.5;

export const std::filesystem::path SHADERS_DIR("shaders");
export const std::filesystem::path DEFAULT_ASSETS_DIR("assets");
export const std::filesystem::path SAVES_DIR("saves");
export const std::filesystem::path SETTINGS_FILE(SAVES_DIR / "settings.json");
export const std::filesystem::path SAVE_DATA_FILE(SAVES_DIR / "save.json");

export const Shape::Sphere DEFAULT_COLLIDER { .radius = 0.5 };

}

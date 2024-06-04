#include <GL/glew.h>
#include <SDL_video.h>
#include <algorithm>
#include <compare>
#include <filesystem>
#include <optional>
#include <utility>
#include <vector>

#include "src/cameras/gui_camera.h"
#include "src/cameras/ortho_camera.h"
#include "src/consts.h"
#include "src/gui/game_screen_factory.h"
#include "src/loader/level_loader.h"
#include "src/loader/levels_meta_loader.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/sound/sound.h"
#include "src/systems/calculate_world_bbox.h"
#include "src/systems/camera.h"
#include "src/systems/cursor.h"
#include "src/systems/enemy.h"
#include "src/systems/finish_condition.h"
#include "src/systems/particles.h"
#include "src/systems/player.h"
#include "src/systems/projectiles.h"
#include "src/systems/tutorial_buttons.h"
#include "src/systems/update_gui.h"
#include "src/systems/velocity.h"

void
load_next_level(const Events::LoadLevelEvent&)
{
  auto& current_level = app().info().current_level;
  auto levels_meta = load_levels_meta(LEVELS_DIR / "levels.json");
  if (levels_meta.levels.empty()) {
    logger().error("No levels found");
    return;
  }
  std::vector<std::filesystem::path>::iterator next_level_it;
  if (current_level.has_value()) {
    auto found_level_it = std::find(levels_meta.levels.begin(),
                                    levels_meta.levels.end(),
                                    current_level.value());
    next_level_it = found_level_it + 1;
  } else {
    next_level_it = levels_meta.levels.begin();
  }
  if (next_level_it < levels_meta.levels.end()) {
    int width = DEFAULT_SCREEN_WIDTH;
    int height = DEFAULT_SCREEN_HEIGHT;
    SDL_GetWindowSize(&app().window(), &width, &height);
    app().scenes().clear();

    std::unique_ptr<Camera> camera =
      std::make_unique<OrthoCamera>(width, height);
    auto game = std::make_unique<Scene>(std::move(camera));
    game->handlers().once(calculate_world_bbox);
    game->handlers().add(cursor_handler_system);
    game->handlers().add(player_moving_system);
    game->handlers().add(player_rotation_system);
    game->handlers().add(player_shooting_system);
    game->handlers().add(camera_move_system);
    game->handlers().add(enemy_hunting_system);
    game->handlers().add(enemy_rotation_system);
    game->handlers().add(enemy_sinking_system);
    game->handlers().add(particle_handler_system);
    game->handlers().add(velocity_gravity_system);
    game->handlers().add(velocity_system);
    game->handlers().add(projectile_handler_system);
    game->handlers().add(tutorial_buttons_system);
    game->handlers().add(tutorial_buttons_system);
    game->handlers().add(check_finish_condition);
    game->handlers().add(update_gui_calculate_hostiles);
    load_level(LEVELS_DIR / "entities.json", *next_level_it, *game);
    calculate_world_bbox(*game);
    app().info().current_level = *next_level_it;
    app().push_scene(std::move(game));

    std::unique_ptr<Camera> gui_camera =
      std::make_unique<GUICamera>(width, height);
    glViewport(0, 0, width, height);
    auto ui = std::make_unique<Scene>(std::move(gui_camera));
    ui->handlers().once(game_screen_factory);
    ui->handlers().add(update_gui);
    app().push_scene(std::move(ui));
  }
}

void
play_sound(const Events::ShootEvent& sound_event)
{
  cache().get_sound(sound_event.sound_path)->play();
}

void
register_common_handlers()
{
  events<const Events::ShootEvent>().add(play_sound);
  events<const Events::LoadLevelEvent>().add(load_next_level);
  app().add_once_handler(
    [](auto&) { events<const Events::LoadLevelEvent>().emit({}); });
}

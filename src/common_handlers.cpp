#include <compare>
#include <filesystem>
#include <iterator>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>

#include "src/cameras/gui_camera.h"
#include "src/cameras/perspective_camera.h"
#include "src/consts.h"
#include "src/fileio/level_loader.h"
#include "src/fileio/levels_meta_loader.h"
#include "src/fileio/save_data_io.h"
#include "src/gui/game_screen_factory.h"
#include "src/gui/in_game_main_menu_factory.h"
#include "src/gui/loading_factory.h"
#include "src/gui/screens/main_menu_screen.h"
#include "src/scene/scene.h"
#include "src/services.h"
#include "src/sound/sound.h"
#include "src/systems/calculate_world_bbox.h"
#include "src/systems/camera.h"
#include "src/systems/collision.h"
#include "src/systems/cursor.h"
#include "src/systems/debris.h"
#include "src/systems/enemy.h"
#include "src/systems/entities_collector.h"
#include "src/systems/finish_condition.h"
#include "src/systems/gun.h"
#include "src/systems/level.h"
#include "src/systems/particles.h"
#include "src/systems/player.h"
#include "src/systems/projectiles.h"
#include "src/systems/tutorial_buttons.h"
#include "src/systems/update_gui.h"
#include "src/systems/velocity.h"
#include "src/utils/system.h"

std::unique_ptr<Camera>
make_gui_camera(const App& app)
{
  auto screen_size = app.screen_size();
  return std::make_unique<GUICamera>(screen_size.width, screen_size.height);
}

std::unique_ptr<Camera>
make_game_camera(const App& app)
{
  auto screen_size = app.screen_size();
  return std::make_unique<PerspectiveCamera>(screen_size.width,
                                             screen_size.height);
}

void
load_loading_screen()
{
  auto camera = make_gui_camera(Services::app());
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::loading_factory);
  Services::app().push_scene(std::move(scene));
}

void
load_in_game_main_menu()
{
  auto camera = make_gui_camera(Services::app());
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::in_game_main_menu_factory);
  scene->handlers().add(update_gui);
  Services::app().push_scene(std::move(scene));
}

Scene&
load_level_scene(bool is_last_level)
{
  Services::app().scenes().clear();
  load_loading_screen();

  auto camera = make_game_camera(Services::app());
  auto game = std::make_unique<Scene>(std::move(camera));
  game->is_deferred(true);
  game->handlers().once(calculate_world_bbox);
  game->handlers().add(collect_entities_system);

  game->handlers().add(velocity_gravity_system);
  game->handlers().add(acceleration_system);
  game->handlers().add(damping_system);
  game->handlers().add(collision_system);
  game->handlers().add(velocity_system);
  game->handlers().add(level_boundaries_system);

  game->handlers().add(cursor_handler_system);
  game->handlers().add(player_moving_system);
  game->handlers().add(player_rotation_system);
  game->handlers().add(player_shooting_system);
  game->handlers().add(enemy_hunting_system);
  game->handlers().add(enemy_rotation_system);
  game->handlers().add(particle_handler_system);

  game->handlers().add(remove_debris_system);
  game->handlers().add(camera_move_system);
  game->handlers().add(gun_shooting_system);
  game->handlers().add(projectile_handler_system);
  game->handlers().add(tutorial_buttons_system);
  game->handlers().add(tutorial_buttons_system);
  if (!is_last_level) {
    game->handlers().add(check_finish_condition);
  }
  game->handlers().add(update_gui_calculate_hostiles);
  game->cancel_handlers().add([](Scene& scene) {
    scene.is_paused(true);
    Services::app().add_once_handler([](auto&) { load_in_game_main_menu(); });
  });

  auto gui_camera = make_gui_camera(Services::app());
  auto ui = std::make_unique<Scene>(std::move(gui_camera));
  ui->handlers().once(GUI::game_screen_factory);
  ui->handlers().add(update_gui);
  ui->is_deferred(false);

  auto& game_ref = *game;
  Services::app().scenes().clear();
  Services::app().push_scene(std::move(game));
  Services::app().push_scene(std::move(ui));
  return game_ref;
}

void
load_current_level(const Events::LoadCurrentLevelEvent&)
{
  auto exec_path = get_excutable_path();
  auto save_data = load_save_data(exec_path / SAVE_DATA_FILE);
  auto levels_meta = load_levels_meta(LEVELS_DIR / "levels.json");
  if (!save_data.current_level.has_value()) {
    Services::events<const Events::LoadNextLevelEvent>().emit({});
    Services::logger().info(
      "Can't get save data, a default level will be used");
    return;
  }
  auto current_level = std::ranges::find(levels_meta.levels,
                                         save_data.current_level.value());
  if (current_level == levels_meta.levels.end()) {
    Services::events<const Events::LoadNextLevelEvent>().emit({});
    Services::logger().info(
      "The user's progress not found, a default level will be used");
    return;
  }
  auto& scene = load_level_scene(
    std::distance(current_level, levels_meta.levels.end()) <= 1);
  load_level(LEVELS_DIR / "entities.json", *current_level, scene);
  calculate_world_bbox(scene);
  Services::app().info().current_level = *current_level;
}

void
load_next_level(const Events::LoadNextLevelEvent&)
{
  auto& current_level = Services::app().info().current_level;
  auto levels_meta = load_levels_meta(LEVELS_DIR / "levels.json");
  if (levels_meta.levels.empty()) {
    Services::logger().error("No levels found");
    return;
  }
  std::vector<std::filesystem::path>::iterator next_level_it;
  if (current_level.has_value()) {
    auto found_level_it = std::ranges::find(levels_meta.levels,
                                            current_level.value());
    next_level_it = found_level_it + 1;
  } else {
    next_level_it = levels_meta.levels.begin();
  }
  bool is_last_level = std::distance(next_level_it, levels_meta.levels.end())
                       == 1;
  if (next_level_it < levels_meta.levels.end()) {
    auto& scene = load_level_scene(is_last_level);
    load_level(LEVELS_DIR / "entities.json", *next_level_it, scene);
    calculate_world_bbox(scene);
    Services::app().info().current_level = *next_level_it;
    Services::app().save_data().save({ .current_level = *next_level_it });
  }
}

void
play_sound(const Events::ShootEvent& sound_event)
{
  Services::cache().get_sound(sound_event.sound_path)->play();
}

void
register_common_handlers()
{
  Services::events<const Events::ShootEvent>().add(play_sound);
  Services::events<const Events::LoadCurrentLevelEvent>().add(
    load_current_level);
  Services::events<const Events::LoadNextLevelEvent>().add(load_next_level);
  Services::app().add_once_handler([](auto&) {
    auto scene = load_main_menu();
    Services::app().push_scene(std::move(scene));
  });
}

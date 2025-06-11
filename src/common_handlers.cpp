#include <compare>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "src/cameras/gui_camera.h"
#include "src/cameras/perspective_camera.h"
#include "src/consts.h"
#include "src/fileio/level_loader.h"
#include "src/fileio/levels_order_loader.h"
#include "src/fileio/save_data_io.h"
#include "src/gui/game_screen_factory.h"
#include "src/gui/in_game_main_menu_factory.h"
#include "src/gui/loading_factory.h"
#include "src/gui/lose_menu_factory.h"
#include "src/gui/screens/load_credits_screen.h"
#include "src/gui/utils/utils.h"
#include "src/services.h"
#include "src/services/logger.h"
#include "src/sound/sound.h"
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
#include "src/systems/ui.h"
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
  scene->handlers().add(ui_system);
  Services::app().push_scene(std::move(scene));
}

void
load_lose_menu(const Events::LoseEvent&)
{
  auto camera = make_gui_camera(Services::app());
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::lose_menu_factory);
  scene->handlers().add(update_gui);
  scene->handlers().add(ui_system);
  Services::app().push_scene(std::move(scene));
}

Scene&
load_level_scene()
{
  Services::app().scenes().clear();
  load_loading_screen();

  auto camera = make_game_camera(Services::app());
  auto game = std::make_unique<Scene>(std::move(camera));
  game->is_deferred(true);
  game->handlers().once(enemy_initial_rotation);
  game->handlers().add(collect_entities_system);

  game->handlers().add(velocity_gravity_system);
  game->handlers().add(acceleration_system);
  game->handlers().add(damping_system);
  game->handlers().add(collision_system);
  game->handlers().add(velocity_system);
  game->handlers().add(player_updating_app_info_system);
  game->handlers().add(level_boundaries_system);

  game->handlers().add(cursor_handler_system);
  game->handlers().add(player_moving_system);
  game->handlers().add(player_rotation_system);
  game->handlers().add(player_shooting_system);
  game->handlers().add(player_enemy_pointers);
  game->handlers().add(enemy_hunting_system);
  game->handlers().add(enemy_rotation_system);
  game->handlers().add(enemy_acceleration_system);
  game->handlers().add(particle_handler_system);

  game->handlers().add(remove_debris_system);
  game->handlers().add(camera_move_system);
  game->handlers().add(gun_shooting_system);
  game->handlers().add(projectile_handler_system);
  game->handlers().add(check_finish_condition);
  game->handlers().add(LoseSystem {});
  game->handlers().add(update_gui_calculate_hostiles);
  game->handlers().add(update_gui_lives);
  game->cancel_handlers().add([](Scene& scene) {
    scene.is_paused(true);
    Services::app().add_once_handler([](auto&) { load_in_game_main_menu(); });
  });

  auto gui_camera = make_gui_camera(Services::app());
  auto ui = std::make_unique<Scene>(std::move(gui_camera));
  ui->handlers().once(GUI::game_screen_factory);
  ui->handlers().add(update_gui);
  ui->handlers().add(ui_system);
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
  auto levels_dir = Services::app().levels_dir();
  auto levels_order = load_levels_order(levels_dir / "levels.json");
  if (!save_data.current_level.has_value()) {
    Services::events<const Events::LoadNextLevelEvent>().emit({});
    Services::logger().info(
      "Can't get save data, a default level will be used");
    return;
  }
  auto current_level = std::ranges::find(levels_order.levels,
                                         save_data.current_level.value());
  if (current_level == levels_order.levels.end()) {
    Services::events<const Events::LoadNextLevelEvent>().emit({});
    Services::logger().info(
      "The user's progress not found, a default level will be used");
    return;
  }
  auto& scene = load_level_scene();
  load_level(levels_dir / "entities.json", *current_level, scene);
  Services::app().info().current_level = *current_level;
}

void
load_next_level(const Events::LoadNextLevelEvent&)
{
  auto& current_level = Services::app().info().current_level;
  auto levels_dir = Services::app().levels_dir();
  auto levels_order = load_levels_order(levels_dir / "levels.json");
  if (levels_order.levels.empty()) {
    Services::logger().error("No levels found");
    return;
  }
  std::vector<std::filesystem::path>::iterator next_level_it;
  if (current_level.has_value()) {
    auto found_level_it = std::ranges::find(levels_order.levels,
                                            current_level.value());
    next_level_it = found_level_it + 1;
  } else {
    next_level_it = levels_order.levels.begin();
  }
  bool is_game_finished = next_level_it == levels_order.levels.end();
  if (is_game_finished) {
    Services::app().scenes().clear();
    auto scene = load_credits_screen();
    Services::app().push_scene(std::move(scene));
    return;
  }
  if (next_level_it < levels_order.levels.end()) {
    auto& scene = load_level_scene();
    load_level(levels_dir / "entities.json", *next_level_it, scene);
    Services::app().info().current_level = *next_level_it;
    Services::app().save_data().save({ .current_level = *next_level_it });
  }
}

void
play_sound(const Events::ShootEvent& sound_event)
{
  Services::cache().get_sound(sound_event.sound_path)->play(sound_event.volume);
}

void
register_common_handlers()
{
  Services::events<const Events::ShootEvent>().add(play_sound);
  Services::events<const Events::LoadCurrentLevelEvent>().add(
    load_current_level);
  Services::events<const Events::LoseEvent>().add(load_lose_menu);
  Services::events<const Events::LoadNextLevelEvent>().add(load_next_level);
  Services::app().add_once_handler(GUI::go_to_main_menu);
}

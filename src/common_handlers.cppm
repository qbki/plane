module;
#include <compare>
#include <filesystem>
#include <optional>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

export module pln.common_handlers;

import pln.app.app;
import pln.cameras.gui_camera;
import pln.cameras.icamera;
import pln.cameras.perspective_camera;
import pln.consts;
import pln.events.event;
import pln.events.event_emitter;
import pln.fileio.level_loader;
import pln.fileio.levels_order_loader;
import pln.fileio.save_data_io;
import pln.gui.credits_screen_factory;
import pln.gui.game_screen_factory;
import pln.gui.in_game_main_menu_factory;
import pln.gui.loading_factory;
import pln.gui.lose_menu_factory;
import pln.gui.screens.load_credits_screen;
import pln.gui.screens.main_menu_screen;
import pln.gui.utils;
import pln.scene.iscene;
import pln.scene.scene;
import pln.services.app;
import pln.services.cache;
import pln.services.events;
import pln.services.logger;
import pln.systems.camera;
import pln.systems.collision;
import pln.systems.cursor;
import pln.systems.debris;
import pln.systems.enemy;
import pln.systems.entities_collector;
import pln.systems.finish_condition;
import pln.systems.gun;
import pln.systems.level;
import pln.systems.particles;
import pln.systems.player;
import pln.systems.projectiles;
import pln.systems.ui;
import pln.systems.update_gui;
import pln.systems.velocity;
import pln.utils.system;

using namespace pln::events;
using namespace pln::fileio;
using namespace pln::gui::utils;
using namespace pln::gui;

namespace pln::common_handlers {

std::unique_ptr<pln::cameras::ICamera>
make_gui_camera(const pln::app::App& app)
{
  auto screen_size = app.screen_size();
  return std::make_unique<pln::cameras::GUICamera>(screen_size.width,
                                                   screen_size.height);
}

std::unique_ptr<pln::cameras::ICamera>
make_game_camera(const pln::app::App& app)
{
  auto screen_size = app.screen_size();
  return std::make_unique<pln::cameras::PerspectiveCamera>(screen_size.width,
                                                           screen_size.height);
}

void
load_loading_screen()
{
  auto camera = make_gui_camera(pln::services::app());
  auto scene = std::make_unique<pln::scene::Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(pln::gui::loading_factory);
  pln::services::app().push_scene(std::move(scene));
}

void
load_in_game_main_menu()
{
  auto camera = make_gui_camera(pln::services::app());
  auto scene = std::make_unique<pln::scene::Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(in_game_main_menu_factory);
  scene->handlers().add(pln::systems::update_gui::update_gui);
  scene->handlers().add(pln::systems::ui::ui);
  pln::services::app().push_scene(std::move(scene));
}

void
load_lose_menu(const LoseEvent&)
{
  auto camera = make_gui_camera(pln::services::app());
  auto scene = std::make_unique<pln::scene::Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(pln::gui::lose_menu_factory);
  scene->handlers().add(pln::systems::update_gui::update_gui);
  scene->handlers().add(pln::systems::ui::ui);
  pln::services::app().push_scene(std::move(scene));
}

pln::scene::IScene&
load_level_scene()
{
  pln::services::app().scenes().clear();
  load_loading_screen();

  auto camera = make_game_camera(pln::services::app());
  auto game = std::make_unique<pln::scene::Scene>(std::move(camera));
  game->is_deferred(true);
  game->handlers().once(pln::systems::enemy::enemy_initial_rotation);
  game->handlers().add(pln::systems::entities_collector::collect_entities);

  game->handlers().add(pln::systems::velocity::velocity_gravity);
  game->handlers().add(pln::systems::velocity::acceleration);
  game->handlers().add(pln::systems::velocity::damping);
  game->handlers().add(pln::systems::collision::collision);
  game->handlers().add(pln::systems::velocity::velocity);
  game->handlers().add(pln::systems::player::player_updating_app_info);
  game->handlers().add(pln::systems::level::level_boundaries);

  game->handlers().add(pln::systems::cursor::cursor_handler);
  game->handlers().add(pln::systems::player::player_moving);
  game->handlers().add(pln::systems::player::player_rotation);
  game->handlers().add(pln::systems::player::player_shooting);
  game->handlers().add(pln::systems::player::player_enemy_pointers);
  game->handlers().add(pln::systems::enemy::enemy_hunting);
  game->handlers().add(pln::systems::enemy::enemy_rotation);
  game->handlers().add(pln::systems::enemy::enemy_acceleration);
  game->handlers().add(pln::systems::particles::particle_handler);

  game->handlers().add(pln::systems::debris::remove_debris);
  game->handlers().add(pln::systems::camera::camera_movement);
  game->handlers().add(pln::systems::gun::gun_shooting);
  game->handlers().add(pln::systems::projectiles::projectile_handler);
  game->handlers().add(pln::systems::finish_condition::check_finish_condition);
  game->handlers().add(pln::systems::player::LoseSystem {});
  game->handlers().add(pln::systems::update_gui::update_gui_calculate_hostiles);
  game->handlers().add(pln::systems::update_gui::update_gui_lives);
  game->cancel_handlers().add([](pln::scene::IScene& scene) {
    scene.is_paused(true);
    pln::services::app().add_once_handler([](auto&) { load_in_game_main_menu(); });
  });

  auto gui_camera = make_gui_camera(pln::services::app());
  auto ui = std::make_unique<pln::scene::Scene>(std::move(gui_camera));
  ui->handlers().once(game_screen_factory);
  ui->handlers().add(pln::systems::update_gui::update_gui);
  ui->handlers().add(pln::systems::ui::ui);
  ui->is_deferred(false);

  auto& game_ref = *game;
  pln::services::app().scenes().clear();
  pln::services::app().push_scene(std::move(game));
  pln::services::app().push_scene(std::move(ui));
  return game_ref;
}

void
load_current_level(const LoadCurrentLevelEvent&)
{
  auto exec_path = utils::system::get_excutable_path();
  auto save_data = load_save_data(exec_path / pln::consts::SAVE_DATA_FILE,
                                  pln::services::app().assets_dir());
  auto levels_dir = pln::services::app().levels_dir();
  auto levels_order = load_levels_order(levels_dir / "levels.json");
  if (!save_data.current_level.has_value()) {
    pln::services::events<const LoadNextLevelEvent>().emit({});
    pln::services::logger().info(
      "Can't get save data, a default level will be used");
    return;
  }
  auto current_level = std::ranges::find(levels_order.levels,
                                         save_data.current_level.value());
  if (current_level == levels_order.levels.end()) {
    pln::services::events<const LoadNextLevelEvent>().emit({});
    pln::services::logger().info(
      "The user's progress not found, a default level will be used");
    return;
  }
  auto& scene = load_level_scene();
  load_level(levels_dir / "entities.json", *current_level, scene);
  pln::services::app().info().current_level = *current_level;
}

void
load_next_level(const LoadNextLevelEvent&)
{
  auto& current_level = pln::services::app().info().current_level;
  auto levels_dir = pln::services::app().levels_dir();
  auto levels_order = load_levels_order(levels_dir / "levels.json");
  if (levels_order.levels.empty()) {
    pln::services::logger().error("No levels found");
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
    pln::services::app().scenes().clear();
    auto scene = pln::gui::screens::load_credits_screen();
    pln::services::app().push_scene(std::move(scene));
    return;
  }
  if (next_level_it < levels_order.levels.end()) {
    auto& scene = load_level_scene();
    load_level(levels_dir / "entities.json", *next_level_it, scene);
    pln::services::app().info().current_level = *next_level_it;
    pln::services::app().save_data().save(
        pln::services::app().assets_dir(),
        { .current_level = *next_level_it });
  }
}

void
play_sound(const ShootEvent& sound_event)
{
  pln::services::cache().get_sound(sound_event.sound_path)->play(sound_event.volume);
}

export
void
register_common_handlers()
{
  pln::services::events<const ShootEvent>().add(play_sound);
  pln::services::events<const LoadCurrentLevelEvent>().add(
    load_current_level);
  pln::services::events<const LoseEvent>().add(load_lose_menu);
  pln::services::events<const LoadNextLevelEvent>().add(load_next_level);
  pln::services::app().add_once_handler(pln::gui::screens::go_to_main_menu);
}

}

#define GLM_ENABLE_EXPERIMENTAL
#include <cstdlib>
#include <format>
#include <string>
#include <utility>

#include "consts.h"
#include "fileio/theme_loader.h"
#include "fileio/translation_loader.h"
#include "game_loop.h"
#include "logger/logger.h"
#include "mesh.h"
#include "sdl_init.h"
#include "services.h"
#include "src/services/logger.h"
#include "systems/render.h"
#include "utils/file_loaders/file_loaders.h"
#include "utils/types.h"

import common_handlers;
import control;
import pln;
import utils.system;

int
main(int argc, char* argv[])
{
  pln::Service<AbstractLogger>::install(std::make_unique<Logger>());
  pln::Service<Cache>::install(std::make_unique<Cache>());
  pln::Service<Events::EventEmitter<const Events::ShootEvent>>::install(
    std::make_unique<Events::EventEmitter<const Events::ShootEvent>>());
  pln::Service<Events::EventEmitter<const Events::LoadNextLevelEvent>>::install(
    std::make_unique<Events::EventEmitter<const Events::LoadNextLevelEvent>>());
  pln::Service<Events::EventEmitter<const Events::LoseEvent>>::install(
    std::make_unique<Events::EventEmitter<const Events::LoseEvent>>());
  pln::Service<Events::EventEmitter<const Events::LoadCurrentLevelEvent>>::install(
    std::make_unique<
      Events::EventEmitter<const Events::LoadCurrentLevelEvent>>());

  auto window = init_window(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  auto context = init_context(window.get());
  auto control = std::make_unique<pln::control::Control>();

  auto geometry_pass_shader = std::make_unique<Shader>();
  {
    auto vertex = load_text(SHADERS_DIR / "main_v.glsl").or_crash();
    auto fragment = load_text(SHADERS_DIR / "deferred_geometry_pass_f.glsl")
                      .or_crash();
    geometry_pass_shader->compile(vertex, fragment);
  }

  auto light_pass_shader = std::make_unique<Shader>();
  {
    auto vertex = load_text(SHADERS_DIR / "output_v.glsl").or_crash();
    auto fragment = load_text(SHADERS_DIR / "deferred_light_pass_f.glsl")
                      .or_crash();
    light_pass_shader->compile(vertex, fragment);
  }

  auto particle_shader = std::make_unique<Shader>();
  {
    auto vertex = load_text(SHADERS_DIR / "main_v.glsl").or_crash();
    auto fragment = load_text(SHADERS_DIR / "particle_f.glsl").or_crash();
    particle_shader->compile(vertex, fragment);
  }

  auto ui_shader = std::make_unique<Shader>();
  {
    auto vertex = load_text(SHADERS_DIR / "main_v.glsl").or_crash();
    auto fragment = load_text(SHADERS_DIR / "particle_f.glsl").or_crash();
    ui_shader->compile(vertex, fragment);
  }

  auto inter_shader = std::make_unique<Shader>();
  {
    auto vertex = load_text(SHADERS_DIR / "output_v.glsl").or_crash();
    auto fragment = load_text(SHADERS_DIR / "output_f.glsl").or_crash();
    inter_shader->compile(vertex, fragment);
  }

  auto deferred_shading = std::make_unique<DeferredShading>(
    std::move(geometry_pass_shader),
    std::move(light_pass_shader),
    create_quad(1.0, 1.0, FARTHEST_NDS_Z_COORD),
    DEFAULT_SCREEN_WIDTH,
    DEFAULT_SCREEN_HEIGHT);

  auto vg_canvas = init_vg_canvas(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

  auto assets_dir = pln::utils::system::get_excutable_path() / DEFAULT_ASSETS_DIR;
  if (argc == 2) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    assets_dir = std::filesystem::path(std::string(argv[1]));
  }
  if (System::is_pc) {
    if (!is_file_exists(assets_dir)) {
      Services::logger().error(
        std::format("Assets directory was not found {}", assets_dir.string()));
      return 1;
    }
  }
  Services::logger().info(
    std::format("Used assets directory: {}", assets_dir.string()));

  auto app = std::make_unique<App>(assets_dir);
  app->gl_context(std::move(context));
  app->control(std::move(control));
  app->deferred_shading(std::move(deferred_shading));
  app->particle_shader(std::move(particle_shader));
  app->ui_shader(std::move(ui_shader));
  app->intermediate_shader(std::move(inter_shader));
  app->window(std::move(window));
  app->validate();

  app->add_handler([](App& app) {
    for (auto& scene : app.scenes()) {
      scene->update();
    }
  });
  app->add_handler(render_system);

  auto translations = load_translations(app->assets_dir() / "text" / "en.json");
  app->set_translations(translations);

  pln::Service<App>::install(std::move(app));

  auto theme = load_theme(Services::app().levels_dir() / "theme.json");
  pln::Service<const GUI::Theme>::install(std::move(theme));

  auto quad = std::make_unique<Services::GuiQuad>(create_quad(1, 1, 0));
  pln::Service<const Services::GuiQuad>::install(std::move(quad));

  pln::common_handlers::register_common_handlers();
  game_loop();
}

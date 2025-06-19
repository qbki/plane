#define GLM_ENABLE_EXPERIMENTAL
#include <cstdlib>
#include <format>
#include <string>
#include <utility>

#include "fileio/theme_loader.h"
#include "fileio/translation_loader.h"
#include "logger/logger.h"
#include "shading/deferred_shading.h"
#include "utils/types.h"

import pln.app.app;
import pln.cache;
import pln.common_handlers;
import pln.consts;
import pln.control;
import pln.events.event;
import pln.events.event_emitter;
import pln.game_loop;
import pln.logger;
import pln.sdl;
import pln.service;
import pln.services.app;
import pln.services.gui_quad;
import pln.services.logger;
import pln.shaders;
import pln.shapes;
import pln.systems.renderer;
import pln.utils.file_loaders;
import pln.utils.platform;
import pln.utils.system;

using namespace pln::utils::file_loaders;

int
main(int argc, char* argv[])
{
  pln::Service<pln::logger::AbstractLogger>::install(std::make_unique<Logger>());

  pln::Service<pln::cache::Cache>::install(std::make_unique<pln::cache::Cache>());

  pln::Service<pln::events::EventEmitter<const pln::events::ShootEvent>>::install(
    std::make_unique<pln::events::EventEmitter<const pln::events::ShootEvent>>());

  pln::Service<pln::events::EventEmitter<const pln::events::LoadNextLevelEvent>>::install(
    std::make_unique<pln::events::EventEmitter<const pln::events::LoadNextLevelEvent>>());

  pln::Service<pln::events::EventEmitter<const pln::events::LoseEvent>>::install(
    std::make_unique<pln::events::EventEmitter<const pln::events::LoseEvent>>());

  pln::Service<pln::events::EventEmitter<const pln::events::LoadCurrentLevelEvent>>::install(
    std::make_unique<
      pln::events::EventEmitter<const pln::events::LoadCurrentLevelEvent>>());

  auto window = pln::sdl::init_window(pln::consts::DEFAULT_SCREEN_WIDTH,
                                      pln::consts::DEFAULT_SCREEN_HEIGHT);
  auto context = pln::sdl::init_context(window.get());
  auto control = std::make_unique<pln::control::Control>();

  auto geometry_pass_shader = std::make_unique<pln::shaders::Shader>();
  {
    auto vertex = load_text(pln::consts::SHADERS_DIR / "main_v.glsl").or_crash();
    auto fragment = load_text(pln::consts::SHADERS_DIR / "deferred_geometry_pass_f.glsl")
                      .or_crash();
    geometry_pass_shader->compile(vertex, fragment);
  }

  auto light_pass_shader = std::make_unique<pln::shaders::Shader>();
  {
    auto vertex = load_text(pln::consts::SHADERS_DIR / "output_v.glsl").or_crash();
    auto fragment = load_text(pln::consts::SHADERS_DIR / "deferred_light_pass_f.glsl")
                      .or_crash();
    light_pass_shader->compile(vertex, fragment);
  }

  auto particle_shader = std::make_unique<pln::shaders::Shader>();
  {
    auto vertex = load_text(pln::consts::SHADERS_DIR / "main_v.glsl").or_crash();
    auto fragment = load_text(pln::consts::SHADERS_DIR / "particle_f.glsl").or_crash();
    particle_shader->compile(vertex, fragment);
  }

  auto ui_shader = std::make_unique<pln::shaders::Shader>();
  {
    auto vertex = load_text(pln::consts::SHADERS_DIR / "main_v.glsl").or_crash();
    auto fragment = load_text(pln::consts::SHADERS_DIR / "particle_f.glsl").or_crash();
    ui_shader->compile(vertex, fragment);
  }

  auto inter_shader = std::make_unique<pln::shaders::Shader>();
  {
    auto vertex = load_text(pln::consts::SHADERS_DIR / "output_v.glsl").or_crash();
    auto fragment = load_text(pln::consts::SHADERS_DIR / "output_f.glsl").or_crash();
    inter_shader->compile(vertex, fragment);
  }

  auto deferred_shading = std::make_unique<DeferredShading>(
    std::move(geometry_pass_shader),
    std::move(light_pass_shader),
    pln::shapes::create_quad(1.0, 1.0, pln::consts::FARTHEST_NDS_Z_COORD),
    pln::consts::DEFAULT_SCREEN_WIDTH,
    pln::consts::DEFAULT_SCREEN_HEIGHT);

  auto vg_canvas = pln::sdl::init_vg_canvas(pln::consts::DEFAULT_SCREEN_WIDTH,
                                            pln::consts::DEFAULT_SCREEN_HEIGHT);

  auto assets_dir = pln::utils::system::get_excutable_path() / pln::consts::DEFAULT_ASSETS_DIR;
  if (argc == 2) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    assets_dir = std::filesystem::path(std::string(argv[1]));
  }
  if (pln::utils::platform::IS_PC) {
    if (!is_file_exists(assets_dir)) {
      pln::services::logger().error(
        std::format("Assets directory was not found {}", assets_dir.string()));
      return 1;
    }
  }
  pln::services::logger().info(
    std::format("Used assets directory: {}", assets_dir.string()));

  auto app = std::make_unique<pln::app::App>(assets_dir);
  app->gl_context(std::move(context));
  app->control(std::move(control));
  app->deferred_shading(std::move(deferred_shading));
  app->particle_shader(std::move(particle_shader));
  app->ui_shader(std::move(ui_shader));
  app->intermediate_shader(std::move(inter_shader));
  app->window(std::move(window));
  app->validate();

  app->add_handler([](pln::app::App& app) {
    for (auto& scene : app.scenes()) {
      scene->update();
    }
  });
  app->add_handler(pln::systems::renderer::render);

  auto translations = load_translations(app->assets_dir() / "text" / "en.json");
  app->set_translations(translations);

  pln::Service<pln::app::App>::install(std::move(app));

  auto theme = load_theme(pln::services::app().levels_dir() / "theme.json");
  pln::Service<const GUI::Theme>::install(std::move(theme));

  auto quad = std::make_unique<pln::services::GuiQuad>(pln::shapes::create_quad(1, 1, 0));
  pln::Service<const pln::services::GuiQuad>::install(std::move(quad));

  pln::common_handlers::register_common_handlers();
  pln::game_loop();
}

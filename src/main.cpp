#define GLM_ENABLE_EXPERIMENTAL
#include <filesystem>
#include <format>
#include <string>
#include <utility>

#include "app/app_builder.h"
#include "common_handlers.h"
#include "consts.h"
#include "fileio/theme_loader.h"
#include "fileio/translation_loader.h"
#include "game_loop.h"
#include "logger/logger.h"
#include "mesh.h"
#include "scene/scene.h"
#include "sdl_init.h"
#include "service.h"
#include "services.h"
#include "systems/render.h"
#include "utils/file_loaders/file_loaders.h"
#include "utils/system.h"
#include "utils/types.h"

int
main(int argc, char* argv[])
{
  Service<AbstractLogger>::install(std::make_unique<Logger>());
  Service<Cache>::install(std::make_unique<Cache>());
  Service<Events::EventEmitter<const Events::ShootEvent>>::install(
    std::make_unique<Events::EventEmitter<const Events::ShootEvent>>());
  Service<Events::EventEmitter<const Events::LoadNextLevelEvent>>::install(
    std::make_unique<Events::EventEmitter<const Events::LoadNextLevelEvent>>());
  Service<Events::EventEmitter<const Events::LoseEvent>>::install(
    std::make_unique<Events::EventEmitter<const Events::LoseEvent>>());
  Service<Events::EventEmitter<const Events::LoadCurrentLevelEvent>>::install(
    std::make_unique<
      Events::EventEmitter<const Events::LoadCurrentLevelEvent>>());

  auto window = init_window(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  auto context = init_context(window.get());
  auto control = std::make_unique<Control>();

  auto output_vertex_shader = load_text(SHADERS_DIR / "output_v.glsl")
                                .or_crash();

  auto geometry_pass_shader = std::make_unique<Shader>();
  auto geometry_pass_vertex_shader = load_text(SHADERS_DIR / "main_v.glsl")
                                       .or_crash();
  auto geometry_pass_fragment_shader = load_text(
                                         SHADERS_DIR
                                         / "deferred_geometry_pass_f.glsl")
                                         .or_crash();
  geometry_pass_shader->compile(geometry_pass_vertex_shader,
                                geometry_pass_fragment_shader);

  auto light_pass_shader = std::make_unique<Shader>();
  auto light_pass_fragment_shader = load_text(SHADERS_DIR
                                              / "deferred_light_pass_f.glsl")
                                      .or_crash();
  light_pass_shader->compile(output_vertex_shader, light_pass_fragment_shader);

  auto particle_shader = std::make_unique<Shader>();
  auto particle_vertex_shader = load_text(SHADERS_DIR / "main_v.glsl")
                                  .or_crash();
  auto particle_fragment_shader = load_text(SHADERS_DIR / "particle_f.glsl")
                                    .or_crash();
  particle_shader->compile(particle_vertex_shader, particle_fragment_shader);

  auto inter_shader = std::make_unique<Shader>();
  auto inter_fragment_shader = load_text(SHADERS_DIR / "output_f.glsl")
                                 .or_crash();
  inter_shader->compile(output_vertex_shader, inter_fragment_shader);

  auto deferred_shading = std::make_unique<DeferredShading>(
    std::move(geometry_pass_shader),
    std::move(light_pass_shader),
    Mesh::quad(1.0, 1.0, FARTHEST_NDS_Z_COORD),
    DEFAULT_SCREEN_WIDTH,
    DEFAULT_SCREEN_HEIGHT);

  AppBuilder app_builder = AppBuilder();
  app_builder.screen_size(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  app_builder.context(std::move(context));
  app_builder.control(std::move(control));
  app_builder.deferred_shading(std::move(deferred_shading));
  app_builder.particle_shader(std::move(particle_shader));
  app_builder.intermediate_shader(std::move(inter_shader));
  app_builder.window(std::move(window));

  auto assets_dir = get_excutable_path() / DEFAULT_ASSETS_DIR;
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

  app_builder.assets_dir(assets_dir);

  std::unique_ptr<App> app { app_builder.build() };

  app->add_handler([](App& app) {
    for (auto& scene : app.scenes()) {
      scene->update();
    }
  });
  app->add_handler(render_system);

  auto translations = load_translations(app->assets_dir() / "text" / "en.json");
  app->set_translations(translations);

  Service<App>::install(std::move(app));

  auto theme = load_theme(Services::app().levels_dir() / "theme.json");
  Service<const GUI::Theme>::install(std::move(theme));

  auto quad = std::make_unique<Services::GuiQuad>(Mesh::quad(HALF, HALF, 0));
  Service<const Services::GuiQuad>::install(std::move(quad));

  register_common_handlers();
  game_loop();
}

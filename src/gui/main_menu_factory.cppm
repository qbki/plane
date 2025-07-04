module;
#include <utility>

export module pln.gui.main_menu_factory;

import pln.components.common;
import pln.events.event;
import pln.fileio.save_data_io;
import pln.gui.components.ui;
import pln.gui.screens.settings_screen;
import pln.gui.utils;
import pln.scene.iscene;
import pln.services.app;
import pln.services.events;
import pln.utils.platform;

using namespace pln::components;
using namespace pln::events;
using namespace pln::fileio;
using namespace pln::gui::components;
using namespace pln::gui::utils;

namespace pln::gui {

export
void
main_menu_factory(const pln::scene::IScene& scene)
{
  auto registry = scene.state().shared_registry();
  auto ui = make_ui(registry);
  auto children = Children({});
  auto assets_dir = pln::services::app().assets_dir();
  auto save_data = pln::services::app().save_data().load(assets_dir);

  if (save_data.current_level.has_value()) {
    auto continue_button_entity = ui.text_button({
      .text = "Continue",
      .on_pointer_down_once =
        [](auto&) {
          pln::services::app().add_once_handler([](auto&) {
            pln::services::events<const LoadCurrentLevelEvent>().emit({});
          });
        },
    });
    children.value.push_back(continue_button_entity);
  }

  auto new_button_entity = ui.text_button({
    .text = "New Game",
    .on_pointer_down_once =
      [](auto&) {
        pln::services::app().add_once_handler([](auto&) {
          clear_user_progress(pln::services::app());
          pln::services::events<const LoadNextLevelEvent>().emit({});
        });
      },
  });
  children.value.push_back(new_button_entity);

  auto settings_button_entity = ui.text_button({
    .text = "Settings",
    .on_pointer_down =
      [](auto&) {
        pln::services::app().add_once_handler([](auto&) {
          pln::services::app().pause_scenes();
          auto scene = pln::gui::screens::load_settings_screen();
          pln::services::app().push_scene(std::move(scene));
        });
      },
  });
  children.value.push_back(settings_button_entity);

  if (pln::utils::platform::IS_PC) {
    auto exit_button_entity = ui.text_button({
      .text = "Exit",
      .on_pointer_down_once = [](auto&) { pln::services::app().is_running(false); },
    });
    children.value.push_back(exit_button_entity);
  }

  ui.block({ .children = children });
}

}

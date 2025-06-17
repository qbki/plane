#include <string>
#include <utility>

#include "src/components/common.h"
#include "src/fileio/save_data_io.h"
#include "src/gui/components/ui.h"
#include "src/gui/screens/settings_screen.h"
#include "src/gui/utils/utils.h"

#include "main_menu_factory.h"

import pln.scene.iscene;
import pln.services.app;
import pln.services.events;
import pln.utils.platform;

namespace GUI {

void
main_menu_factory(const pln::scene::IScene& scene)
{
  auto registry = scene.state().shared_registry();
  auto ui = Factory::make_ui(registry);
  auto children = Children({});
  auto save_data = pln::services::app().save_data().load();

  if (save_data.current_level.has_value()) {
    auto continue_button_entity = ui.text_button({
      .text = "Continue",
      .on_pointer_down_once =
        [](auto&) {
          pln::services::app().add_once_handler([](auto&) {
            pln::services::events<const Events::LoadCurrentLevelEvent>().emit({});
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
          pln::services::events<const Events::LoadNextLevelEvent>().emit({});
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
          auto scene = load_settings_screen();
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

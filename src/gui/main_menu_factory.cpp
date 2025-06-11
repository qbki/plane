#include <string>
#include <utility>

#include "src/components/common.h"
#include "src/fileio/save_data_io.h"
#include "src/gui/components/ui.h"
#include "src/gui/screens/settings_screen.h"
#include "src/gui/utils/utils.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "main_menu_factory.h"

namespace GUI {

void
main_menu_factory(const Scene& scene)
{
  auto registry = scene.state().shared_registry();
  auto ui = Factory::make_ui(registry);
  auto children = Children({});
  auto save_data = Services::app().save_data().load();

  if (save_data.current_level.has_value()) {
    auto continue_button_entity = ui.text_button({
      .text = "Continue",
      .on_pointer_down_once =
        [](auto&) {
          Services::app().add_once_handler([](auto&) {
            Services::events<const Events::LoadCurrentLevelEvent>().emit({});
          });
        },
    });
    children.value.push_back(continue_button_entity);
  }

  auto new_button_entity = ui.text_button({
    .text = "New Game",
    .on_pointer_down_once =
      [](auto&) {
        Services::app().add_once_handler([](auto&) {
          clear_user_progress(Services::app());
          Services::events<const Events::LoadNextLevelEvent>().emit({});
        });
      },
  });
  children.value.push_back(new_button_entity);

  auto settings_button_entity = ui.text_button({
    .text = "Settings",
    .on_pointer_down =
      [](auto&) {
        Services::app().add_once_handler([](auto&) {
          Services::app().pause_scenes();
          auto scene = load_settings_screen();
          Services::app().push_scene(std::move(scene));
        });
      },
  });
  children.value.push_back(settings_button_entity);

  if (System::is_pc) {
    auto exit_button_entity = ui.text_button({
      .text = "Exit",
      .on_pointer_down_once = [](auto&) { Services::app().is_running(false); },
    });
    children.value.push_back(exit_button_entity);
  }

  ui.block({ .children = children });
}

}

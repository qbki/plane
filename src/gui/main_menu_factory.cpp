#include <filesystem>
#include <utility>

#include "src/components/common.h"
#include "src/gui/components/ui.h"
#include "src/gui/screens/settings_screen.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "main_menu_factory.h"

namespace GUI {

void
main_menu_factory(const Scene& scene)
{
  auto ui = Factory::make_ui(scene.state().shared_registry());
  auto children = Children({});

  auto new_button_entity = ui.text_button({
    .text = "New Game",
    .on_pointer_down_once =
      [](auto&) {
        Services::app().add_once_handler([](auto&) {
          Services::app().info().current_level = std::nullopt;
          Services::events<const Events::LoadLevelEvent>().emit({});
        });
      },
  });
  children.value.push_back(new_button_entity);

  auto settings_button_entity = ui.text_button({
    .text = "Settings",
    .on_pointer_down =
      [](auto&) {
        Services::app().add_once_handler([](auto&) {
          for (auto& scene : Services::app().scenes()) {
            scene->is_paused(true);
          }
          auto scene = load_settings_screen();
          Services::app().push_scene(std::move(scene));
        });
      },
  });
  children.value.push_back(settings_button_entity);

  if (Services::app().system().is_pc) {
    auto exit_button_entity = ui.text_button({
      .text = "Exit",
      .on_pointer_down_once = [](auto&) { Services::app().is_running(false); },
    });
    children.value.push_back(exit_button_entity);
  }

  ui.block({ .children = children });
}

}

#include <utility>
#include <vector>

#include "src/cameras/gui_camera.h"
#include "src/gui/settings_screen_factory.h"
#include "src/services.h"
#include "src/systems/update_gui.h"

#include "settings_screen.h"

std::unique_ptr<Scene>
load_settings_screen()
{
  auto screen_size = Services::app().screen_size();
  auto camera = std::make_unique<GUICamera>(screen_size.width,
                                            screen_size.height);
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::settings_screen_factory);
  scene->handlers().add(update_gui);

  scene->cancel_handlers().add([](auto&) {
    Services::app().add_once_handler([](auto&) {
      Services::app().pop_scene();
      auto scenes_quantity = Services::app().scenes().size();
      if (scenes_quantity > 0) {
        Services::app().scenes()[scenes_quantity - 1]->is_paused(false);
      }
    });
  });

  return scene;
}

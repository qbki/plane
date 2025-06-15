#include <utility>
#include <vector>

#include "src/cameras/gui_camera.h"
#include "src/gui/settings_screen_factory.h"
#include "src/systems/ui.h"
#include "src/systems/update_gui.h"

#include "settings_screen.h"

import pln.services.app;

std::unique_ptr<Scene>
load_settings_screen()
{
  auto screen_size = pln::services::app().screen_size();
  auto camera = std::make_unique<GUICamera>(screen_size.width,
                                            screen_size.height);
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::settings_screen_factory);
  scene->handlers().add(update_gui);
  scene->handlers().add(ui_system);

  scene->cancel_handlers().add([](auto&) {
    pln::services::app().add_once_handler([](auto&) {
      pln::services::app().pop_scene();
      auto scenes_quantity = pln::services::app().scenes().size();
      if (scenes_quantity > 0) {
        pln::services::app().scenes()[scenes_quantity - 1]->is_paused(false);
      }
    });
  });

  return scene;
}

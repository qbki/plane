#include <utility>
#include <vector>

#include "src/cameras/gui_camera.h"
#include "src/gui/settings_screen_factory.h"

#include "settings_screen.h"

import pln.scene.iscene;
import pln.scene.scene;
import pln.services.app;
import pln.systems.ui;
import pln.systems.update_gui;

std::unique_ptr<pln::scene::IScene>
load_settings_screen()
{
  auto screen_size = pln::services::app().screen_size();
  auto camera = std::make_unique<GUICamera>(screen_size.width,
                                            screen_size.height);
  auto scene = std::make_unique<pln::scene::Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::settings_screen_factory);
  scene->handlers().add(pln::systems::update_gui::update_gui);
  scene->handlers().add(pln::systems::ui::ui);

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

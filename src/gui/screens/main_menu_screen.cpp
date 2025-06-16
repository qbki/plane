#include <utility>

#include "src/cameras/gui_camera.h"
#include "src/gui/main_menu_factory.h"

#include "main_menu_screen.h"

import pln.services.app;
import pln.systems.ui;
import pln.systems.update_gui;

std::unique_ptr<Scene>
load_main_menu()
{
  auto screen_size = pln::services::app().screen_size();
  auto camera = std::make_unique<GUICamera>(screen_size.width,
                                            screen_size.height);
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::main_menu_factory);
  scene->handlers().add(pln::systems::update_gui::update_gui);
  scene->handlers().add(pln::systems::ui::ui);
  return scene;
}

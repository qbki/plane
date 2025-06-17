#include <utility>

#include "src/gui/main_menu_factory.h"

#include "main_menu_screen.h"

import pln.cameras.gui_camera;
import pln.scene.iscene;
import pln.scene.scene;
import pln.services.app;
import pln.systems.ui;
import pln.systems.update_gui;

std::unique_ptr<pln::scene::IScene>
load_main_menu()
{
  auto screen_size = pln::services::app().screen_size();
  auto camera = std::make_unique<pln::cameras::GUICamera>(screen_size.width,
                                                          screen_size.height);
  auto scene = std::make_unique<pln::scene::Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::main_menu_factory);
  scene->handlers().add(pln::systems::update_gui::update_gui);
  scene->handlers().add(pln::systems::ui::ui);
  return scene;
}

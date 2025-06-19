#include <utility>

#include "load_credits_screen.h"

import pln.cameras.gui_camera;
import pln.gui.credits_screen_factory;
import pln.scene.iscene;
import pln.scene.scene;
import pln.services.app;
import pln.systems.update_gui;

std::unique_ptr<pln::scene::IScene>
load_credits_screen()
{
  auto screen_size = pln::services::app().screen_size();
  auto camera = std::make_unique<pln::cameras::GUICamera>(screen_size.width,
                                                          screen_size.height);
  auto scene = std::make_unique<pln::scene::Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(pln::gui::credits_screen_factory);
  scene->handlers().add(pln::systems::update_gui::update_gui);
  return scene;
}

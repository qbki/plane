#include <utility>

#include "src/cameras/gui_camera.h"
#include "src/gui/credits_screen_factory.h"
#include "src/systems/update_gui.h"

#include "load_credits_screen.h"

import pln.services.app;

std::unique_ptr<Scene>
load_credits_screen()
{
  auto screen_size = pln::services::app().screen_size();
  auto camera = std::make_unique<GUICamera>(screen_size.width,
                                            screen_size.height);
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::credits_screen_factory);
  scene->handlers().add(update_gui);
  return scene;
}

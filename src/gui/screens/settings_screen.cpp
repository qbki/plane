#include <utility>

#include "src/cameras/gui_camera.h"
#include "src/gui/settings_screen_factory.h"
#include "src/services.h"
#include "src/systems/update_gui.h"

#include "settings_screen.h"

std::unique_ptr<Scene>
load_settings_screen()
{
  auto screen_size = Services::app().screen_size();
  auto camera =
    std::make_unique<GUICamera>(screen_size.width, screen_size.height);
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::settings_screen_factory);
  scene->handlers().add(update_gui);
  return scene;
}

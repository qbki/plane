#include <utility>

#include "src/cameras/gui_camera.h"
#include "src/gui/main_menu_factory.h"
#include "src/services.h"
#include "src/systems/ui.h"
#include "src/systems/update_gui.h"

#include "main_menu_screen.h"

std::unique_ptr<Scene>
load_main_menu()
{
  auto screen_size = Services::app().screen_size();
  auto camera = std::make_unique<GUICamera>(screen_size.width,
                                            screen_size.height);
  auto scene = std::make_unique<Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(GUI::main_menu_factory);
  scene->handlers().add(update_gui);
  scene->handlers().add(ui_system);
  return scene;
}

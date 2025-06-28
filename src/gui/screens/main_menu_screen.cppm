module;
#include <memory>
#include <utility>

export module pln.gui.screens.main_menu_screen;

import pln.app.app;
import pln.cameras.gui_camera;
import pln.gui.main_menu_factory;
import pln.scene.iscene;
import pln.scene.scene;
import pln.services.app;
import pln.systems.ui;
import pln.systems.update_gui;

namespace pln::gui::screens {

export
std::unique_ptr<pln::scene::IScene>
load_main_menu()
{
  auto screen_size = pln::services::app().screen_size();
  auto camera = std::make_unique<pln::cameras::GUICamera>(screen_size.width,
                                                          screen_size.height);
  auto scene = std::make_unique<pln::scene::Scene>(std::move(camera));
  scene->is_deferred(false);
  scene->handlers().once(pln::gui::main_menu_factory);
  scene->handlers().add(pln::systems::update_gui::update_gui);
  scene->handlers().add(pln::systems::ui::ui);
  return scene;
}

export
void
go_to_main_menu(pln::app::App& app)
{
  app.scenes().clear();
  auto scene = load_main_menu();
  app.push_scene(std::move(scene));
}

}

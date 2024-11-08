#include <utility>
#include <vector>

#include "src/app/app.h"
#include "src/gui/screens/main_menu_screen.h"
#include "src/scene/scene.h"

#include "utils.h"

namespace GUI {

void
setup_main_menu(App& app)
{
  app.scenes().clear();
  auto scene = load_main_menu();
  app.push_scene(std::move(scene));
}

}

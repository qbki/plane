module;
#include <sstream>
#include <string>

export module pln.gui.credits_screen_factory;

import pln.components.common;
import pln.gui.components.ui;
import pln.gui.utils;
import pln.scene.iscene;
import pln.services.app;
import pln.services.theme;
import pln.utils.file_loaders;

using namespace pln::gui::utils;
using namespace pln::utils::file_loaders;

namespace pln::gui {

std::vector<std::string>
split_by_line(const std::string& value)
{
  std::vector<std::string> result;
  std::stringstream ss(value);
  std::string line;
  while (std::getline(ss, line)) {
    result.push_back(line);
  }
  return result;
}

export
void
credits_screen_factory(const pln::scene::IScene& scene)
{
  auto ui = components::make_ui(scene.state().shared_registry());
  auto credits_file = pln::services::app().credits_file();
  auto credits_txt = load_text(credits_file).or_fallback("");
  auto credits_lines = split_by_line(credits_txt);
  pln::components::Children children { {} };

  for (const auto& line : credits_lines) {
    children.value.push_back(ui.text({
      .font { pln::services::theme().typography.h3 },
      .text { line == "" ? " " : line },
    }));
  }

  children.value.push_back(ui.text_button({
    .text = "Go to title sreen",
    .on_pointer_down =
      [](auto&) {
        clear_user_progress(pln::services::app());
        pln::services::app().add_once_handler(go_to_main_menu);
      },
  }));

  ui.block({ .children = children });
}

}

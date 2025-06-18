#include <sstream>
#include <string>

#include "src/gui/components/ui.h"
#include "src/gui/utils/utils.h"
#include "src/utils/file_loaders/file_loaders.h"

#include "credits_screen_factory.h"

import pln.components.common;
import pln.scene.iscene;
import pln.services.app;
import pln.services.theme;

namespace GUI {

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

void
credits_screen_factory(const pln::scene::IScene& scene)
{
  auto ui = Factory::make_ui(scene.state().shared_registry());
  auto credits_file = pln::services::app().credits_file();
  auto credits_txt = load_text(credits_file).or_fallback("");
  auto credits_lines = split_by_line(credits_txt);
  pln::components::Children children { {} };

  for (const auto& line : credits_lines) {
    children.value.push_back(ui.text({
      .font = pln::services::theme().typography.h3,
      .text = (line == "") ? " " : line,
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

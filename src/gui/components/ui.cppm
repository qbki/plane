module;
#include <entt/entt.hpp>
#include <functional>
#include <memory>

export module pln.gui.components.ui;

import pln.gui.components.background;
import pln.gui.components.block;
import pln.gui.components.div;
import pln.gui.components.div;
import pln.gui.components.progress;
import pln.gui.components.rect;
import pln.gui.components.row;
import pln.gui.components.text;
import pln.gui.components.text_button;

namespace pln::gui::components {

export
struct UIComponents
{
  template<typename T>
  using UIComponent = std::function<entt::entity(const T& config)>;

  UIComponent<BackgroundConfig> background;
  UIComponent<BlockConfig> block;
  UIComponent<DivConfig> div;
  UIComponent<ProgressConfig> progress;
  UIComponent<RectConfig> rect;
  UIComponent<RowConfig> row;
  UIComponent<TextButtonConfig> text_button;
  UIComponent<TextConfig> text;
};


export
UIComponents
make_ui(std::shared_ptr<entt::registry>& registry)
{
  return {
    .background =
      [registry](const auto& config) mutable {
        return background(registry, config);
      },
    .block =
      [registry](const auto& config) mutable {
        return block(registry, config);
      },
    .div =
      [registry](const auto& config) mutable {
        return div(registry, config);
      },
    .progress =
      [registry](const auto& config) mutable {
        return progress(registry, config);
      },
    .rect =
      [registry](const auto& config) mutable {
        return rect(registry, config);
      },
    .row =
      [registry](const auto& config) mutable {
        return row(registry, config);
      },
    .text_button =
      [registry](const auto& config) mutable {
        return text_button(registry, config);
      },
    .text =
      [registry](const auto& config) mutable {
        return text(registry, config);
      },
  };
}

}

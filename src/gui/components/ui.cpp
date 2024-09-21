#include "background.h"
#include "block.h"
#include "progress.h"
#include "rect.h"
#include "row.h"
#include "text.h"
#include "text_button.h"

#include "ui.h"

namespace GUI::Factory {

UIComponents
make_ui(std::shared_ptr<entt::registry>& registry)
{
  return {
    .background =
      [registry](const auto& config) mutable {
        return Factory::background(registry, config);
      },
    .block =
      [registry](const auto& config) mutable {
        return Factory::block(registry, config);
      },
    .progress =
      [registry](const auto& config) mutable {
        return Factory::progress(registry, config);
      },
    .rect =
      [registry](const auto& config) mutable {
        return Factory::rect(registry, config);
      },
    .row =
      [registry](const auto& config) mutable {
        return Factory::row(registry, config);
      },
    .text_button =
      [registry](const auto& config) mutable {
        return Factory::text_button(registry, config);
      },
    .text =
      [registry](const auto& config) mutable {
        return Factory::text(registry, config);
      },
  };
}

}

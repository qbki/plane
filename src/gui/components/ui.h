#pragma once
#include <entt/entt.hpp>
#include <functional>
#include <memory>

#include "block.h"
#include "progress.h"
#include "rect.h"
#include "src/gui/components/background.h"
#include "src/gui/components/row.h"
#include "text.h"
#include "text_button.h"

namespace GUI::Factory {

struct UIComponents
{
  template<typename T>
  using UIComponent = std::function<entt::entity(const T& config)>;

  UIComponent<Factory::BackgroundConfig> background;
  UIComponent<Factory::BlockConfig> block;
  UIComponent<Factory::ProgressConfig> progress;
  UIComponent<Factory::RectConfig> rect;
  UIComponent<Factory::RowConfig> row;
  UIComponent<Factory::TextButtonConfig> text_button;
  UIComponent<Factory::TextConfig> text;
};

UIComponents
make_ui(std::shared_ptr<entt::registry>& registry);

}

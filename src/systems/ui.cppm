module;
#include <thorvg.h>
#include <utility>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/gui/ui_canvas.h"
#include "src/scene/scene.h"

export module pln.systems.ui;

import pln.shapes;
import pln.utils.color;

namespace pln::systems::ui {

export
void
ui(const Scene& scene)
{
  auto& registry = scene.state().shared_registry();
  registry->view<pln::shapes::RectSize, pln::utils::color::Color, Transform, IsDirty, UIRect>().each(
    [&](entt::entity entity, const pln::shapes::RectSize& rect,
        const pln::utils::color::Color& color,
        Transform& transform,
        IsDirty& is_dirty) {
      if (is_dirty.value) {
        auto shape = tvg::Shape::gen();
        shape->appendRect(0,
                          0,
                          static_cast<float>(rect.width),
                          static_cast<float>(rect.height));
        shape->fill(color.r, color.g, color.b);
        shape->opacity(color.a);

        transform.scale({ rect.width, rect.height, 1 });

        UiCanvas canvas(rect.width, rect.height);
        canvas.canvas().push(std::move(shape));

        registry->replace<UiCanvas>(entity, std::move(canvas));
      }
    });
}

}

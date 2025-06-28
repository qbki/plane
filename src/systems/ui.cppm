module;
#include <entt/entt.hpp>
#include <thorvg.h>
#include <utility>

export module pln.systems.ui;

import pln.components.common;
import pln.components.transform;
import pln.gui.ui_canvas;
import pln.scene.iscene;
import pln.shapes;
import pln.utils.color;

using namespace pln::components;

namespace pln::systems::ui {

export
void
ui(const pln::scene::IScene& scene)
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

        pln::gui::UiCanvas canvas(rect.width, rect.height);
        canvas.canvas().push(std::move(shape));

        registry->replace<pln::gui::UiCanvas>(entity, std::move(canvas));
      }
    });
}

}

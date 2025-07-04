module;
#include <entt/entt.hpp>
#include <optional>

export module pln.gui.components.progress;

import pln.components.common;
import pln.components.percent;
import pln.components.transform;
import pln.consts;
import pln.events.event;
import pln.events.event_emitter;
import pln.gui.components.div;
import pln.gui.components.rect;
import pln.math.shapes;

using namespace pln::components;
using namespace pln::events;

namespace pln::gui::components {

export
struct ProgressConfig
{
  float width { 100.f }; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  float height { 10.f }; // NOLINT(cppcoreguidelines-avoid-magic-numbers)
  pln::components::Parent parent { std::nullopt };
  int value { 0 };
};


export
entt::entity
progress(std::shared_ptr<entt::registry>& registry,
         const ProgressConfig& config)
{
  const float line_height_coefficient = 0.1f;

  auto line = rect(registry,
                   {
                     .width = config.width,
                     .height = config.height * line_height_coefficient,
                   });

  auto progress = rect(registry,
                       {
                         .width = config.width,
                         .height = config.height,
                       });

  auto entity = div(registry,
                    {
                      .width = config.width,
                      .height = config.height,
                      .parent = config.parent,
                      .children = Children({ line, progress }),
                    });

  registry->emplace<Percent>(entity, config.value);
  registry->emplace<IsDirty>(entity, true);

  auto& layout = registry->emplace<EventEmitter<GUILayout>>(entity);

  layout.add([registry, entity, line, progress](auto&) {
    auto [percent, rect, is_dirty] = registry->get<Percent, pln::math::RectSize, IsDirty>(
      entity);
    if (!is_dirty.value) {
      return;
    }
    auto [progress_size,
          is_progress_dirty] = registry->get<pln::math::RectSize, IsDirty>(progress);
    progress_size.width = static_cast<int>(rect.width * percent.norm());

    auto [line_transform, line_size] = registry->get<Transform, pln::math::RectSize>(line);
    auto line_position = line_transform.translation();
    line_position.y = static_cast<float>(progress_size.height
                                         - line_size.height)
                      * pln::consts::HALF;
    line_transform.translate(line_position);

    is_progress_dirty.value = true;
    is_dirty.value = false;
  });

  return entity;
}

}

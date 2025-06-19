#include "src/events/event_emitter.h"
#include "src/gui/components/div.h"
#include "src/gui/components/rect.h"

#include "progress.h"

import pln.components.common;
import pln.components.percent;
import pln.components.transform;
import pln.consts;
import pln.shapes;

using namespace pln::components;

namespace GUI::Factory {

entt::entity
progress(std::shared_ptr<entt::registry>& registry,
         const ProgressConfig& config)
{
  const float line_height_coefficient = 0.1f;

  auto line = Factory::rect(registry,
                            {
                              .width = config.width,
                              .height = config.height * line_height_coefficient,
                            });

  auto progress = Factory::rect(registry,
                                {
                                  .width = config.width,
                                  .height = config.height,
                                });

  auto entity = Factory::div(registry,
                             {
                               .width = config.width,
                               .height = config.height,
                               .parent = config.parent,
                               .children = Children({ line, progress }),
                             });

  registry->emplace<Percent>(entity, config.value);
  registry->emplace<IsDirty>(entity, true);

  auto& layout = registry->emplace<Events::EventEmitter<Events::GUILayout>>(
    entity);

  layout.add([registry, entity, line, progress](auto&) {
    auto [percent, rect, is_dirty] = registry->get<Percent, pln::shapes::RectSize, IsDirty>(
      entity);
    if (!is_dirty.value) {
      return;
    }
    auto [progress_size,
          is_progress_dirty] = registry->get<pln::shapes::RectSize, IsDirty>(progress);
    progress_size.width = static_cast<int>(rect.width * percent.norm());

    auto [line_transform, line_size] = registry->get<Transform, pln::shapes::RectSize>(line);
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

#include "src/components/common.h"
#include "src/components/percent.h"
#include "src/events/event_emitter.h"
#include "src/gui/components/div.h"
#include "src/gui/components/rect.h"
#include "src/shapes.h"

#include "progress.h"

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

  auto& layout =
    registry->emplace<Events::EventEmitter<Events::GUILayout>>(entity);

  layout.add(
    [registry, entity, line, progress, line_height_coefficient](auto&) {
      auto [percent, rect] = registry->get<Percent, RectSize>(entity);
      auto& line_rect = registry->get<RectSize>(line);
      auto& progress_rect = registry->get<RectSize>(progress);
      progress_rect.width = static_cast<int>(rect.width * percent.value());
      progress_rect.height = rect.height;
      line_rect.width = rect.width;
      line_rect.height = static_cast<int>(static_cast<float>(rect.height) *
                                          line_height_coefficient);
    });

  return entity;
}

}

module;
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "src/components/common.h"
#include "src/components/transform.h"
#include "src/math/intersection.h"
#include "src/scene/scene.h"

export module pln.systems.update_gui;

import pln.services.app;
import pln.shapes;
import pln.utils.ecs;
import pln.utils.mouse;

namespace pln::systems::update_gui {

export
void
update_gui(Scene& scene)
{
  glm::ivec2 mouse_pos = pln::utils::mouse::mouse_position();
  glm::ivec2 position { mouse_pos.x, mouse_pos.y };
  Events::GUILayout gui_layout_event_data {};
  Events::PointerMove pointer_move_event_data { .position = position };
  Events::PointerEnter pointer_enter_event_data {};
  Events::PointerLeave pointer_leave_event_data {};
  Events::PointerDown pointer_down_event_data { .position = position };
  auto& registry = scene.state().registry();

  registry.view<Events::EventEmitter<Events::GUILayout>>().each(
    [&gui_layout_event_data](
      Events::EventEmitter<Events::GUILayout>& event_emitter) {
      event_emitter.emit(gui_layout_event_data);
    });

  registry
    .view<Events::EventEmitter<Events::PointerMove>,
          Events::EventEmitter<Events::PointerEnter>,
          Events::EventEmitter<Events::PointerLeave>,
          Events::EventEmitter<Events::PointerDown>,
          Transform,
          pln::shapes::RectSize,
          Parent,
          IsPointerInside,
          IsPointerDownEventAccepted>()
    .each([&registry,
           &pointer_move_event_data,
           &pointer_enter_event_data,
           &pointer_leave_event_data,
           &pointer_down_event_data](
            Events::EventEmitter<Events::PointerMove>& pointer_move,
            Events::EventEmitter<Events::PointerEnter>& pointer_enter,
            Events::EventEmitter<Events::PointerLeave>& pointer_leave,
            Events::EventEmitter<Events::PointerDown>& pointer_down,
            Transform& transform,
            pln::shapes::RectSize& rect_size,
            Parent& parent,
            IsPointerInside& was_pointer_inside,
            IsPointerDownEventAccepted& is_down_event_accepted) {
      auto global_matrix = pln::utils::ecs::get_global_matrix(registry, parent);
      auto is_pointer_down = pln::services::app().control().pointer_pressed;
      auto is_pointer_up = !is_pointer_down;
      auto point = global_matrix * glm::vec4(transform.translation(), 1);
      pln::shapes::Rect<int> rect {
        .x = static_cast<int>(point.x),
        .y = static_cast<int>(point.y),
        .width = rect_size.width,
        .height = rect_size.height,
      };
      auto is_pointer_inside = is_inside(rect,
                                         pointer_move_event_data.position);
      if (is_pointer_inside) {
        pointer_move.emit(pointer_move_event_data);
      }
      if (!was_pointer_inside.value && is_pointer_inside) {
        pointer_enter.emit(pointer_enter_event_data);
        was_pointer_inside.value = true;
      } else if (was_pointer_inside.value && !is_pointer_inside) {
        pointer_leave.emit(pointer_leave_event_data);
        was_pointer_inside.value = false;
      }
      auto should_emit_down_event = is_pointer_down
                                    && is_down_event_accepted.value
                                    && is_pointer_inside;
      if (should_emit_down_event) {
        pointer_down.emit(pointer_down_event_data);
      }
      is_down_event_accepted.value = is_pointer_inside && is_pointer_up;
    });
}


export
void
update_gui_calculate_hostiles(const Scene& scene)
{
  int enemy_quantity = 0;
  scene.state().registry().view<const EnemyStateEnum>().each(
    [&enemy_quantity](const EnemyStateEnum& state) {
      if (state == EnemyStateEnum::HUNTING) {
        enemy_quantity++;
      }
    });
  pln::services::app().info().hostiles = enemy_quantity;
}


export
void
update_gui_lives(const Scene& scene)
{
  scene.state().shared_registry()->view<Lives, PlayerKind>().each(
    [](const Lives& lives) { pln::services::app().info().lives = lives; });
}

}

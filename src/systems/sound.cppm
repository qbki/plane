module;
#include <entt/entt.hpp>
export module pln.systems.sound;

import pln.components.common;
import pln.components.transform;
import pln.scene.iscene;
import pln.sounds.sound_listener;
import pln.sounds.sound_source;

namespace pln::systems::sound {

export
void
listener_handler(pln::scene::IScene& scene)
{
  auto &registry = scene.state().registry();
  registry
    .view<components::Transform,
          components::Velocity,
          components::PlayerKind>()
    .each([](const components::Transform& transform,
             const components::Velocity velocity) {
        sounds::set_listener(transform.translation(),
                             velocity.value,
                             transform.up(),
                             transform.forward());
    });
}


export
void
destroy_unused_sounds(pln::scene::IScene& scene)
{
  auto& registry = scene.state().registry();
  registry
    .view<sounds::SoundSource>()
    .each([&registry](entt::entity entity, const sounds::SoundSource& sound_source) {
        if (!sound_source.is_playing()) {
          registry.destroy(entity);
        }
    });
}

}

module;
#include <entt/entt.hpp>
#include <memory>

#include "src/game_state/state.h"

export module pln.scene.iscene;

import pln.collections.octree;
import pln.events.event_emitter;

using namespace pln::events;

namespace pln::scene {

export
class IScene
{
public:
  IScene() = default;
  IScene(const IScene&) = delete;
  IScene& operator=(const IScene&) = delete;
  IScene(const IScene&&) = delete;
  IScene& operator=(IScene&&) = delete;
  virtual ~IScene() = default;

  virtual void update() = 0;
  virtual EventEmitter<IScene>& handlers() = 0;
  virtual EventEmitter<IScene>& cancel_handlers() = 0;
  [[nodiscard]] virtual State& state() const = 0;

  [[nodiscard]] virtual bool is_deferred() const = 0;
  virtual void is_deferred(bool value) = 0;

  [[nodiscard]] virtual bool is_paused() const = 0;
  virtual void is_paused(bool value) = 0;

  virtual std::unique_ptr<pln::collections::Octree<entt::entity>>& entities() = 0;
  virtual void entities(std::unique_ptr<pln::collections::Octree<entt::entity>> value) = 0;
};

}

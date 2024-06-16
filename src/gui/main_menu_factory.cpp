#include "src/components/common.h"
#include "src/gui/components/block.h"
#include "src/gui/components/text_button.h"
#include "src/scene/scene.h"
#include "src/services.h"

#include "main_menu_factory.h"

namespace GUI {

void
main_menu_factory(Scene& scene)
{
  auto& shared_registry = scene.state().shared_registry();

  auto block = Factory::make_block(shared_registry);
  auto new_button_entity =
    GUI::Factory::make_text_button(shared_registry, "New Game");
  shared_registry->replace<Parent>(new_button_entity, block);

  auto& new_pointer_down =
    shared_registry->get<Events::EventEmitter<Events::PointerDown>>(
      new_button_entity);
  new_pointer_down.once([](auto&) {
    Services::app().add_once_handler([](auto&) {
      Events::LoadLevelEvent dummy;
      Services::events<const Events::LoadLevelEvent>().emit(dummy);
    });
  });

  auto& block_children = shared_registry->get<Children>(block);
  block_children.value.push_back(new_button_entity);

  if (Services::app().system().is_pc) {
    auto exit_button_entity =
      GUI::Factory::make_text_button(shared_registry, "Exit");
    shared_registry->replace<Parent>(exit_button_entity, block);
    auto& exit_pointer_down =
      shared_registry->get<Events::EventEmitter<Events::PointerDown>>(
        exit_button_entity);
    exit_pointer_down.once(
      [shared_registry](auto&) { Services::app().is_running(false); });
    block_children.value.push_back(exit_button_entity);
  }
}

}

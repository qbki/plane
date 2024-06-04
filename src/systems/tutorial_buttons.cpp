#include "src/components/common.h"
#include "src/components/textures.h"
#include "src/services.h"
#include "src/utils/noop.h"

#include "tutorial_buttons.h"

void
change_texture_type(Textures& textures, bool is_button_pressed)
{
  auto texture_type = is_button_pressed ? TextureType::Type::SECONDARY
                                        : TextureType::Type::PRIMARY;
  textures.change_type(texture_type);
}

void
tutorial_buttons_system(Scene& scene)
{
  auto& registry = scene.state().registry();
  const auto& control = app().control();
  registry.view<Textures, TutorialButton, TutorialButtonKind>().each(
    [&](Textures& textures, const TutorialButton& button) {
      switch (button.value) {
        case Control::Action::UP:
          change_texture_type(textures, control.up);
          break;
        case Control::Action::DOWN:
          change_texture_type(textures, control.down);
          break;
        case Control::Action::LEFT:
          change_texture_type(textures, control.left);
          break;
        case Control::Action::RIGHT:
          change_texture_type(textures, control.right);
          break;
        case Control::Action::SHOOTING:
          change_texture_type(textures, control.is_player_shooting);
          break;
        default:
          noop();
      }
    });
}

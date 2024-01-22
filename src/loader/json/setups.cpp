#include "setups.h"
#include "extractors.h"

void
setup_player(const nlohmann::basic_json<>& json_strategy,
             App& app,
             entt::entity entity)
{
  auto behaviour = json_strategy.at("behaviour").get<std::string>();
  if (behaviour == "player") {
    auto& game_state = app.game_state;
    game_state->player_id(entity);
  }
}

void
setup_camera(const nlohmann::basic_json<>& json_level, App& app)
{
  auto position = extract_vec3(json_level.at("camera").at("position"));
  app.game_state->camera()->position(position);
}

#include <entt/entity/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>
#include <ranges>
#include <string>

#include "src/components/common.h"
#include "src/components/velocity.h"
#include "src/utils/common.h"
#include "src/utils/file_loaders.h"
#include "src/utils/random.h"

#include "level_loader.h"

glm::vec3
extract_vec3(const nlohmann::basic_json<>& node)
{
  return { node.at(0).get<float>(),
           node.at(1).get<float>(),
           node.at(2).get<float>() };
}

glm::vec3
calc_spread_angle(float z_rotation)
{
  const auto spread_arc_fraction = 16.0;
  auto max_spread = glm::pi<float>() / spread_arc_fraction;
  const auto half_max_spread = max_spread / 2.0;
  const auto percentage_offset = (std::rand() % 100) * 0.01;
  auto spread = max_spread * percentage_offset;
  return { 0.0, 0.0, z_rotation - half_max_spread + spread };
}

struct ExtractVelocityResult
{
  float acceleration;
  float speed;
  float damping;
};
ExtractVelocityResult
extract_velocity(const nlohmann::basic_json<>& node)
{
  auto velocity_node = node.at("velocity");
  return { .acceleration = velocity_node.contains("acceleration")
                             ? velocity_node.at("acceleration").get<float>()
                             : 0,
           .speed = velocity_node.contains("speed")
                      ? velocity_node.at("speed").get<float>()
                      : 0,
           .damping = velocity_node.at("damping") };
}

template<typename T>
std::vector<T>
extract_vector(const nlohmann::basic_json<>& node)
{
  std::vector<T> result;
  std::ranges::copy(node | std::ranges::views::transform(
                             [](auto& v) { return v.template get<T>(); }),
                    std::back_inserter(result));
  return result;
}

struct ParticleParams
{
  unsigned int quantity{};
  ExtractVelocityResult velocity;
  float lifetime{};
};

void
emit_particles(App& app,
               glm::vec3 initial_position,
               const ParticleParams& params,
               const std::string& file_path)
{
  auto& registry = app.game_state->registry();
  unsigned int idx = 0;
  auto step = glm::two_pi<float>() / static_cast<float>(params.quantity);
  auto particles = registry.view<Position,
                                 Rotation,
                                 Lifetime,
                                 LifetimeMax,
                                 Velocity,
                                 Speed,
                                 ParticleKind>(entt::exclude<Available>);

  for (auto [id, position, rotation, lifetime, lifetime_max, velocity, speed] :
       particles.each()) {
    position.value = initial_position;
    rotation.value = { 0.0, 0.0, static_cast<float>(idx) * step };
    auto move_direction = calc_z_direction(rotation.value);
    velocity.velocity = move_direction * speed.value;
    registry.emplace<Available>(id);

    lifetime.value = lifetime_max.value;
    idx += 1;
    if (idx >= (params.quantity - 1)) {
      break;
    }
  }

  for (; idx < (params.quantity - 1); idx += 1) {
    auto entity = app.game_state->factory().make_particle(registry, file_path);
    auto rotation = glm::vec3(0.0, 0.0, static_cast<float>(idx) * step);
    registry.replace<Lifetime>(entity, params.lifetime);
    registry.replace<LifetimeMax>(entity, params.lifetime);
    registry.replace<Position>(entity, initial_position);
    registry.replace<Speed>(entity, params.velocity.speed);
    registry.replace<Rotation>(entity, rotation);
    registry.replace<Velocity>(
      entity,
      Velocity(calc_z_direction(rotation) * params.velocity.speed,
               params.velocity.damping));
  }
}

struct ProjectileParams
{
  ExtractVelocityResult velocity;
  float range;
};

void
emit_projectile(App& app,
                const ProjectileParams& params,
                const std::string& file_path)
{
  auto& registry = app.game_state->registry();

  auto [player_position, player_rotation] =
    app.game_state->player<Position, Rotation>();
  auto projectiles_view =
    registry.view<ProjectileKind>(entt::exclude<Available>);
  auto rotation = calc_spread_angle(player_rotation.value.z);
  auto projectile_id = projectiles_view.front();
  auto move_direction = calc_z_direction(rotation);
  if (projectile_id == entt::null) {
    auto entity =
      app.game_state->factory().make_projectile(registry, file_path);
    registry.replace<Position>(entity, player_position.value);
    registry.replace<Rotation>(entity, rotation);
    registry.replace<InitialPosition>(entity, player_position.value);
    registry.replace<Speed>(entity, params.velocity.speed);
    registry.replace<Range>(entity, params.range);
    registry.replace<Velocity>(
      entity, move_direction * params.velocity.speed, params.velocity.damping);
  } else {
    auto [prj_position,
          prj_initial_position,
          prj_rotation,
          prj_velocity,
          prj_speed] =
      registry.get<Position, InitialPosition, Rotation, Velocity, Speed>(
        projectile_id);
    prj_position.value = player_position.value;
    prj_initial_position.value = player_position.value;
    prj_rotation.value = rotation;
    prj_velocity.velocity = move_direction * prj_speed.value;
    registry.emplace_or_replace<Available>(projectile_id);
  }
}

void
attach_velocity(const nlohmann::basic_json<>& node,
                entt::registry& registry,
                entt::entity entity)
{
  if (!node.contains("velocity")) {
    return;
  }
  auto velocity = extract_velocity(node);
  registry.replace<Velocity>(entity, velocity.acceleration, velocity.damping);
}

void
attach_opaque(const nlohmann::basic_json<>& node,
              entt::registry& registry,
              entt::entity entity)
{
  if (!node.contains("is_opaque")) {
    return;
  }
  auto is_opaque = node.at("is_opaque").get<bool>();
  if (is_opaque) {
    registry.emplace<Opaque>(entity);
  }
}

void
attach_position(const nlohmann::basic_json<>& node,
                entt::registry& registry,
                entt::entity entity)
{
  if (!node.contains("position")) {
    return;
  }
  auto position = extract_vec3(node.at("position"));
  registry.replace<Position>(entity, position);
}

void
attach_direction(const nlohmann::basic_json<>& node,
                 entt::registry& registry,
                 entt::entity entity)
{
  if (!node.contains("direction")) {
    return;
  }
  auto direction = extract_vec3(node.at("direction"));
  registry.replace<Direction>(entity, direction);
}

void
attach_color(const nlohmann::basic_json<>& node,
             entt::registry& registry,
             entt::entity entity)
{
  if (!node.contains("color")) {
    return;
  }
  auto color = extract_vec3(node.at("color"));
  registry.replace<Color>(entity, color);
}

void
attach_particles_emmiter_by_hit(const nlohmann::basic_json<>& particle_node,
                                App& app,
                                entt::entity host_entity)
{
  auto& registry{ app.game_state->registry() };
  auto file_path{ particle_node.at("model").get<std::string>() };
  ParticleParams particle_params{
    .quantity = particle_node.at("quantity").get<unsigned int>(),
    .velocity = extract_velocity(particle_node),
    .lifetime = particle_node.at("life_time").get<float>(),
  };
  ParticlesEmitter emitter{ [particle_params, file_path, &app](
                              glm::vec3 position) {
    emit_particles(app, position, particle_params, file_path);
  } };
  registry.emplace<ParticlesEmitter>(host_entity, emitter);
}

void
attach_projectile_emmiter(const nlohmann::basic_json<>& json_entities,
                          const nlohmann::basic_json<>& node,
                          App& app,
                          entt::entity host_entity)
{
  if (!node.contains("gun_id")) {
    return;
  }
  auto gun_node = json_entities.at(node.at("gun_id").get<std::string>());
  auto& registry{ app.game_state->registry() };
  auto projectile_id{ gun_node.at("projectile_id").get<std::string>() };
  auto projectile_node = json_entities.at(projectile_id);
  auto file_path = projectile_node.at("model").get<std::string>();
  ProjectileParams projectile_params{
    .velocity = extract_velocity(gun_node),
    .range = gun_node.at("range").get<float>(),
  };
  ProjectileEmitter emitter{ [projectile_params, file_path, &app]() {
    emit_projectile(app, projectile_params, file_path);
  } };
  registry.emplace<ProjectileEmitter>(host_entity, emitter);
}

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

void
single_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::ModelMakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto json_entity_id = json_strategy.at("entity_id").get<std::string>();
  auto json_entity = json_entities.at(json_entity_id);
  auto model_path = json_entity.at("model").get<std::string>();
  auto entity = maker_fn(registry, model_path);
  attach_color(json_entity, registry, entity);
  attach_opaque(json_entity, registry, entity);
  attach_position(json_strategy, registry, entity);
  attach_velocity(json_entity, registry, entity);
  attach_projectile_emmiter(json_entities, json_entity, app, entity);
  setup_player(json_strategy, app, entity);
}

void
single_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::MakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto json_entity_id = json_strategy.at("entity_id").get<std::string>();
  auto json_entity = json_entities.at(json_entity_id);
  auto entity = maker_fn(registry);
  attach_color(json_entity, registry, entity);
  attach_direction(json_entity, registry, entity);
  attach_opaque(json_entity, registry, entity);
  attach_position(json_strategy, registry, entity);
  attach_velocity(json_entity, registry, entity);
}

void
square_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::ModelMakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto entity_ids = extract_vector<std::string>(json_strategy.at("entity_ids"));
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), entity_ids.size() - 1);
  auto width = json_strategy.at("width").get<int>();
  auto height = json_strategy.at("height").get<int>();
  auto center = extract_vec3(json_strategy.at("center"));
  const auto half = 2.0f;
  auto start_x = center.x - static_cast<float>(width) / half;
  auto start_y = center.y - static_cast<float>(height) / half;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      auto entity_index = get_random_int();
      auto entity_id = entity_ids.at(entity_index);
      auto json_entity = json_entities.at(entity_id);
      auto model_path = json_entity.at("model").get<std::string>();
      auto entity = maker_fn(registry, model_path);
      registry.replace<Position>(entity,
                                 glm::vec3(start_x + static_cast<float>(x),
                                           start_y + static_cast<float>(y),
                                           center.z));
    }
  }
}

void
round_strategy(const nlohmann::basic_json<>& json_entities,
               const nlohmann::basic_json<>& json_strategy,
               App& app,
               ModelFactory::ModelMakerFn maker_fn)
{
  auto& registry = app.game_state->registry();
  auto entity_ids = extract_vector<std::string>(json_strategy.at("entity_ids"));
  std::vector<ExtractVelocityResult> velocity_items;
  std::ranges::copy(entity_ids | std::ranges::views::transform([&](auto& id) {
                      return extract_velocity(json_entities.at(id));
                    }),
                    std::back_inserter(velocity_items));
  auto get_random_int =
    make_random_fn(static_cast<size_t>(0), entity_ids.size() - 1);
  auto radius = json_strategy.at("radius").get<int>();
  auto center = extract_vec3(json_strategy.at("center"));
  auto radius_float = static_cast<float>(radius);
  for (int x = -radius; x < radius; x++) {
    for (int y = -radius; y < radius; y++) {
      auto distance = glm::length(glm::vec2(x, y));
      if (distance < radius_float) {
        auto entity_index = get_random_int();
        auto entity_id = entity_ids.at(entity_index);
        auto json_entity = json_entities.at(entity_id);
        auto model_path = json_entity.at("model").get<std::string>();
        auto velocity = velocity_items.at(entity_index);
        auto entity = maker_fn(registry, model_path);
        registry.replace<Position>(entity,
                                   glm::vec3(center.x + static_cast<float>(x),
                                             center.y + static_cast<float>(y),
                                             center.z));
        registry.replace<Velocity>(
          entity, velocity.acceleration, velocity.damping);
        attach_particles_emmiter_by_hit(
          json_entities.at(json_strategy.at("hit_particle")), app, entity);
      }
    }
  }
}

void
load_level(const std::string& entities_file_path,
           const std::string& level_file_path,
           App& app)
{
  auto json_entities = load_json(entities_file_path).at("entities");
  auto json_level = load_json(level_file_path);

  auto player_maker = [&app](auto& registry, const auto& file_path) -> auto {
    return app.game_state->factory().make_player(registry, file_path);
  };
  auto static_maker = [&app](auto& registry, const auto& file_path) -> auto {
    return app.game_state->factory().make_static(registry, file_path);
  };
  auto enemy_maker = [&app](auto& registry, const auto& file_path) -> auto {
    return app.game_state->factory().make_enemy(registry, file_path);
  };
  auto directional_light_maker = [&app](auto& registry) -> auto {
    return app.game_state->factory().make_directional_light(registry);
  };
  auto point_light_maker = [&app](auto& registry) -> auto {
    return app.game_state->factory().make_point_light(registry);
  };

  setup_camera(json_level, app);

  for (auto& json_strategy : json_level.at("map")) {
    if (json_strategy.at("position_strategy") == "single") {
      if (json_strategy.at("behaviour") == "player") {
        single_strategy(json_entities, json_strategy, app, player_maker);
      }
      if (json_strategy.at("behaviour") == "light") {
        single_strategy(json_entities, json_strategy, app, point_light_maker);
      }
    }
    if (json_strategy.at("position_strategy") == "void") {
      if (json_strategy.at("behaviour") == "light") {
        single_strategy(
          json_entities, json_strategy, app, directional_light_maker);
      }
    }
    if (json_strategy.at("position_strategy") == "square") {
      if (json_strategy.at("behaviour") == "static") {
        square_strategy(json_entities, json_strategy, app, static_maker);
      }
    }
    if (json_strategy.at("position_strategy") == "round") {
      if (json_strategy.at("behaviour") == "enemy") {
        round_strategy(json_entities, json_strategy, app, enemy_maker);
      }
    }
  }
}

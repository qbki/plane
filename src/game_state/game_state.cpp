#include <algorithm>

#include "game_state.h"
#include "particle_emitter.h"
#include "projectile.h"


void GameState::camera(std::shared_ptr<Camera> camera) {
  this->_camera = camera;
}


std::shared_ptr<Camera> GameState::camera() {
  return this->_camera;
}


void GameState::player(Entity player) {
  this->_player = player;
}


GameState::Entity GameState::player() {
  return this->_player;
}


void GameState::cursor(glm::vec3 value) {
  this->_cursor = value;
}


glm::vec3 GameState::cursor() {
  return this->_cursor;
}


void GameState::add_projectiles(Projectiles& xs) {
  this->add_entities(xs, this->_projectiles);
}


GameState::Projectiles& GameState::projectiles() {
  return this->_projectiles;
}


void GameState::add_enemies(Entities& xs) {
  std::transform(
    begin(xs),
    end(xs),
    back_inserter(this->_enemies_state),
    [](auto item) { return EnemyState(item); }
  );
}


GameState::EnemiesState& GameState::enemies_state() {
  return this->_enemies_state;
}


void GameState::add_decoration(Entities& xs) {
  this->add_entities(xs, this->_decorations);
}


GameState::Entities& GameState::decorations() {
  return this->_decorations;
}


void GameState::add_handler(Handler handler) {
  this->_handlers.push_back(handler);
}


void GameState::update(Control& control, float seconds) {
  Meta meta {*this, control, *this->_camera, seconds};
  for (auto& handler : this->_handlers) {
    handler(meta);
  }
}


ParticleEmitter& GameState::particle_emitter() const {
  return *_particle_emitter;
}


void GameState::particle_emitter(std::unique_ptr<ParticleEmitter> value) {
  _particle_emitter = move(value);
}

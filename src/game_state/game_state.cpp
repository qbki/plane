#include "game_state.h"


GameState::GameState() {
  _last_time_point = SDL_GetTicks64();
}


void GameState::camera(std::shared_ptr<Camera> camera) {
  this->_camera = camera;
}


std::shared_ptr<Camera> GameState::camera() {
  return this->_camera;
}


void GameState::cursor(glm::vec3 value) {
  this->_cursor = value;
}


glm::vec3 GameState::cursor() {
  return this->_cursor;
}


void GameState::player_id(entt::entity value) {
  _player_id = value;
}


entt::entity GameState::player_id() {
  return _player_id;
}


entt::registry& GameState::registry() {
  return _registry;
}


ModelFactory& GameState::factory() {
  return _factory;
}


void GameState::add_handler(Handler handler) {
  this->_handlers.push_back(handler);
}


void GameState::update(Control& control, unsigned long time_since_start_of_program) {
  Meta meta {
    *this,
    control,
    *this->_camera,
    static_cast<float>(time_since_start_of_program - _last_time_point) * 0.001f,
    _last_time_point * 0.001f
  };
  _last_time_point = time_since_start_of_program;
  for (auto& handler : this->_handlers) {
    handler(meta);
  }
}

#include "scene.h"
#include "control.h"
#include <algorithm>


void Scene::add_entities(Entities& source, Entities& destination) {
  std::copy(begin(source), end(source), std::back_inserter(destination));
  this->add_children(source);
}


void Scene::camera(std::shared_ptr<Camera> camera) {
  this->_camera = camera;
}


std::shared_ptr<Camera> Scene::camera() {
  return this->_camera;
}


void Scene::player(Entity player) {
  this->_player = player;
  this->add_child(this->_player);
}


Scene::Entity Scene::player() {
  return this->_player;
}


void Scene::cursor(glm::vec3 value) {
  this->_cursor = value;
}


glm::vec3 Scene::cursor() {
  return this->_cursor;
}


void Scene::add_bullets(Entities& xs) {
  this->add_entities(xs, this->_bullets);
}


Scene::Entities& Scene::bullets() {
  return this->_bullets;
}


void Scene::add_enemies(Entities& xs) {
  std::transform(
    begin(xs),
    end(xs),
    back_inserter(this->_enemies_state),
    [](auto item) { return EnemyState(item); }
  );
  this->add_children(xs);
}


Scene::EnemiesState& Scene::enemies_state() {
  return this->_enemies_state;
}


void Scene::add_decoration(Entities& xs) {
  this->add_entities(xs, this->_decorations);
}


Scene::Entities& Scene::decorations() {
  return this->_decorations;
}


void Scene::add_handler(Handler handler) {
  this->_handlers.push_back(handler);
}


void Scene::update(Control& control, float seconds) {
  Meta meta {*this, control, *this->_camera, seconds};
  for (auto& handler : this->_handlers) {
    handler(meta);
  }
}

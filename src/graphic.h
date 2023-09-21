#pragma once
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

#include "camera.h"
#include "sun_light.h"
#include "utils.h"

class Graphic {
protected:
  glm::vec3 _position = zero<glm::vec3>();
  glm::vec3 _velocity = zero<glm::vec3>();
  float _rotation_z = 0.0;
  glm::mat4 _transform = glm::identity<glm::mat4>();
  std::vector<std::shared_ptr<Graphic>> _children = std::vector<std::shared_ptr<Graphic>>();
  bool _is_active = true;

  void update_transform();

public:
  virtual ~Graphic() {};

  void add_child(std::shared_ptr<Graphic> child);

  virtual void draw(
    const Camera& camera,
    const SunLight& light,
    float elapsed_seconds
  ) const;

  glm::vec3 position() const;
  void position(const glm::vec3& position);

  float rotation_z() const;
  void rotation_z(float rotation);

  glm::vec3 velocity() const;
  void velocity(glm::vec3 speed);

  const glm::mat4& transform() const;
  void transform(const glm::mat4& transform);

  bool is_active() const;
  void is_active(bool value);

  void move_in(glm::vec3 direction_normal, float length);
};

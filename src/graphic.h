#pragma once
#include <memory>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

#include "./camera.h"
#include "./sun_light.h"

class Graphic {
protected:
  glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
  float rotation_z = 0.0;
  glm::mat4 transform = glm::identity<glm::mat4>();
  std::vector<std::shared_ptr<Graphic>> children = std::vector<std::shared_ptr<Graphic>>();

  void update_transform();

public:
  virtual ~Graphic() {};

  void add_child(std::shared_ptr<Graphic> child);

  virtual void draw(
    const Camera& camera,
    const SunLight& light,
    float elapsed_seconds
  ) const;

  glm::vec3 get_position() const;
  void set_position(const glm::vec3& position);

  float get_rotation_z() const;
  void set_rotation_z(float rotation);

  const glm::mat4& get_transform() const;
  void set_transform(const glm::mat4& transform);

  void move_in(glm::vec3 direction_normal, float length);
};

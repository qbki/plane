#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

class TurretRotation
{
private:
  float _speed { 0 };
  float _angle { 0 };

public:
  void speed(float value);
  [[nodiscard]] float speed() const;

  void angle(float value);
  [[nodiscard]] float angle() const;

  void rotate(const glm::vec3& forward,
              const glm::vec3& sight,
              float delta_time);
  [[nodiscard]] glm::quat quat() const;
};

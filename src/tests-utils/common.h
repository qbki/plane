// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <glm/vec3.hpp>

void
check_equality(glm::vec3 a, glm::vec3 b, const std::string& message)
{
  INFO(message);
  REQUIRE_THAT(a.x, Catch::Matchers::WithinRel(b.x));
  REQUIRE_THAT(a.y, Catch::Matchers::WithinRel(b.y));
  REQUIRE_THAT(a.z, Catch::Matchers::WithinRel(b.z));
}
// NOLINTEND(cppcoreguidelines-avoid-do-while)

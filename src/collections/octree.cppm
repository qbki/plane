module;
#include <array>
#include <entt/entt.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <vector>

export module pln.collections.octree;

import pln.consts;
import pln.math.intersections;
import pln.math.shapes;

using namespace pln::math;

namespace pln::collections {

/**
 * An implementation of an octree data structure.
 *
 * Limitations:
 * It doesn't expect elements outside of bounding box;
 */
export
template<typename T>
class Octree
{
private:
  static const size_t CHILDREN_SIZE = 8;
  static const size_t DEFAULT_MAX_DEPTH = 8;

  // Template BBOX_{x}_{y}_{z}
  // MIN == BOTTOM == it goes to the negative Infinity
  // MAX == TOP == it goes to the positive Infinity
  // Positive z axis is considered as an up vector
  static const size_t BBOX_MIN_MAX_TOP = 0;
  static const size_t BBOX_MAX_MAX_TOP = 1;
  static const size_t BBOX_MIN_MIN_TOP = 2;
  static const size_t BBOX_MAX_MIN_TOP = 3;
  static const size_t BBOX_MIN_MAX_BOTTOM = 4;
  static const size_t BBOX_MAX_MAX_BOTTOM = 5;
  static const size_t BBOX_MIN_MIN_BOTTOM = 6;
  static const size_t BBOX_MAX_MIN_BOTTOM = 7;

  using ChildrenType = std::array<std::unique_ptr<Octree>,
                                  Octree::CHILDREN_SIZE>;

  std::vector<T> _data;
  std::array<AABB, CHILDREN_SIZE> _partitioning;
  AABB _bbox;
  unsigned int _depth;
  unsigned int _max_depth;
  glm::vec3 _center = { 0.0, 0.0, 0.0 };
  ChildrenType _children;

public:
  Octree();
  explicit Octree(const AABB& bbox,
                  unsigned int max_depth = DEFAULT_MAX_DEPTH,
                  unsigned int depth = 0);
  void insert(Sphere collider, const T& value);
  std::vector<T> at(const AABB& bbox) const;
  void at(const AABB& bbox, std::vector<T>& acc) const;

  std::vector<T> at(const Sphere& sphere) const;
};

template<typename T>
Octree<T>::Octree()
  : _bbox(AABB {})
  , _depth(0)
  , _max_depth(0)
{
}

template<typename T>
Octree<T>::Octree(const AABB& bbox,
                  unsigned int max_depth,
                  unsigned int depth)
  : _bbox(bbox)
  , _depth(depth)
  , _max_depth(max_depth)
  , _center((bbox.min + bbox.max) * pln::consts::HALF)
{
  _partitioning[BBOX_MIN_MIN_BOTTOM] = {
    .min = _bbox.min,
    .max = _center,
  };
  _partitioning[BBOX_MAX_MIN_BOTTOM] = {
    .min = { _center.x, _bbox.min.y, _bbox.min.z },
    .max = { _bbox.max.x, _center.y, _center.z },
  };
  _partitioning[BBOX_MAX_MAX_BOTTOM] = {
    .min = { _center.x, _center.y, _bbox.min.z },
    .max = { _bbox.max.x, _bbox.max.y, _center.z },
  };
  _partitioning[BBOX_MIN_MAX_BOTTOM] = {
    .min = { _bbox.min.x, _center.y, _bbox.min.z },
    .max = { _center.x, _bbox.max.y, _center.z },
  };
  _partitioning[BBOX_MIN_MIN_TOP] = {
    .min = { _bbox.min.x, _bbox.min.y, _center.z },
    .max = { _center.x, _center.y, _bbox.max.z },
  };
  _partitioning[BBOX_MAX_MIN_TOP] = {
    .min = { _center.x, _bbox.min.y, _center.z },
    .max = { _bbox.max.x, _center.y, _bbox.max.z },
  };
  _partitioning[BBOX_MAX_MAX_TOP] = {
    .min = _center,
    .max = _bbox.max,
  };
  _partitioning[BBOX_MIN_MAX_TOP] = {
    .min = { _bbox.min.x, _center.y, _center.z },
    .max = { _center.x, _bbox.max.y, _bbox.max.z },
  };
}

template<typename T>
void
Octree<T>::insert(Sphere collider, const T& value)
{
  const auto collider_size = collider.radius * 2.0f;
  const auto bbox_child_size = (_bbox.max - _bbox.min) * pln::consts::HALF;
  if (collider_size >= bbox_child_size.x || collider_size >= bbox_child_size.y
      || collider_size >= bbox_child_size.z || _depth >= _max_depth
      || on_axis(collider, _bbox.min) || on_axis(collider, _bbox.max)
      || on_axis(collider, _center)) {
    _data.push_back(value);
    return;
  }
  for (size_t i = 0; i < CHILDREN_SIZE; i++) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
    auto& partitioning = _partitioning[i];
    if (is_inside(partitioning, collider.center)) {
      auto& child = _children[i];
      if (!child) {
        child = std::make_unique<Octree>(partitioning, _max_depth, _depth + 1);
      }
      child->insert(collider, value);
    }
  }
}

template<typename T>
std::vector<T>
Octree<T>::at(const AABB& bbox) const
{
  std::vector<T> result;
  at(bbox, result);
  return result;
}

template<typename T>
std::vector<T>
Octree<T>::at(const Sphere& sphere) const
{
  auto bbox = AABB {
    .min = sphere.center - glm::vec3(sphere.radius),
    .max = sphere.center + glm::vec3(sphere.radius),
  };
  return at(bbox);
}

template<typename T>
void
Octree<T>::at(const AABB& bbox, std::vector<T>& acc) const
{
  std::ranges::copy(_data, std::back_inserter(acc));
  if (_depth >= _max_depth) {
    return;
  }
  for (const auto& child : _children) {
    if (child && are_intersected(child->_bbox, bbox)) {
      child->at(bbox, acc);
    }
  }
}

}

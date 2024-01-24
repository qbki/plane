#pragma once
#include <cmath>

/**
 * y = value * smoothing ^ dt
 * value - interpolated value
 * smoothing - rate of decay
 * example:
 *   dt | value = 10 and smoothing = 0.5
 *   ---|-------------------------------
 *   0  | 10
 *   1  | 10 / 2
 *   2  | 10 / 4
 */
template<typename T>
T
damp(T from, float smoothing, float dt)
{
  return from * std::pow(smoothing, dt);
}

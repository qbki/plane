#pragma once

struct RectSize
{
  int width = 0;
  int height = 0;

  RectSize() = default;
  RectSize(int _width, int _height);
};

template<typename T>
struct Rect
{
  T x;
  T y;
  T width;
  T height;
};

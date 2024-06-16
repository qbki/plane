#include "common.h"

std::vector<unsigned char>
get_pixels(int width, int height, unsigned int size)
{
  std::vector<unsigned char> result;
  result.resize(width * height * size, 0);
  return result;
}

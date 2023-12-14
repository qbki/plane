#pragma once
#include <string>
#include <vector>


class Texture {
private:
  unsigned int _type;
  unsigned int to = 0; // texture object

public:
  explicit Texture(unsigned int type, const std::vector<unsigned char>& data);
  ~Texture();

  void use(unsigned int num);

  unsigned int type();
  void type(unsigned int type);
};

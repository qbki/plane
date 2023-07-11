#pragma once
#include <string>
#include <tiny_gltf.h>

int buffer_size();

template<typename T>
void print(T& value);

tinygltf::Model loadModel(const std::string& filename);

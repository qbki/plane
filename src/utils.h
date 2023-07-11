#pragma once
#include <string>
#include <tiny_gltf.h>

int buffer_size();

template<typename T>
void print(T& value);

tinygltf::Model load_mesh(const std::string& filename);

std::string load_text(const std::string &file_name);

#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <tiny_gltf.h>

tinygltf::Model
load_gltf_model(const std::string& file_name);

std::string
load_text(const std::string& file_name);

nlohmann::basic_json<>
load_json(const std::string& file_name);

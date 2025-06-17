#pragma once
#include <string>

import pln.scene.iscene;

void
load_level(const std::string& entities_file_path,
           const std::string& level_file_path,
           pln::scene::IScene& scene);

#pragma once
#include <memory>

import pln.scene.iscene;

std::unique_ptr<pln::scene::IScene>
load_settings_screen();

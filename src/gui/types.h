#pragma once
#include <variant>

#include "hostiles.h"
#include "text.h"

namespace GUI {

using Component = std::variant<Text, Hostiles>;

}

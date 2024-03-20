/**
 * This file helps to avoid these types of errors (clang-tiny):
 *   warning: no header providing "nlohmann::basic_json" is
 *            directly included [misc-include-cleaner]
 *
 * The "nlohmann/json" library doesn't contain "IWYU pragma"
 * comments at this moment. This is a workaround.
 */
#pragma once
#include <nlohmann/json.hpp>     // IWYU pragma: export
#include <nlohmann/json_fwd.hpp> // IWYU pragma: export

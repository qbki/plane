#pragma once
#include <nlohmann/json.hpp>

#include "src/app.h"

void
single_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::MakerFn maker_fn);

void
square_strategy(const nlohmann::basic_json<>& json_entities,
                const nlohmann::basic_json<>& json_strategy,
                App& app,
                ModelFactory::MakerFn maker_fn);

void
round_strategy(const nlohmann::basic_json<>& json_entities,
               const nlohmann::basic_json<>& json_strategy,
               App& app,
               ModelFactory::MakerFn maker_fn);

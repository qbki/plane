#pragma once
#include <format>
#include <unordered_map>

template<typename Key, typename Value>
Value
map_value(Key key, std::unordered_map<Key, Value>& values)
{
  if (values.contains(key)) {
    return values.at(key);
  }
  throw std::runtime_error(std::format("Key \"{}\" was not found", key));
}

template<typename Key, typename Value>
Value
map_value(Key key, std::unordered_map<Key, Value>&& values)
{
  auto moved_values = std::move(values);
  return map_value(key, moved_values);
}

#include <format>

#include "tvg.h"

import utils.crash;

void
vg_verify_or_crash(const std::string& where, const tvg::Result& result)
{
  auto message = [&where](const std::string& value) {
    return std::format("{}: {}", where, value);
  };
  switch (result) {
    case tvg::Result::FailedAllocation:
      utils::crash(message("FailedAllocation"));
    case tvg::Result::InsufficientCondition:
      utils::crash(message("InsufficientCondition"));
    case tvg::Result::InvalidArguments:
      utils::crash(message("InvalidArguments"));
    case tvg::Result::MemoryCorruption:
      utils::crash(message("MemoryCorruption"));
    case tvg::Result::NonSupport:
      utils::crash(message("NonSupport"));
    case tvg::Result::Unknown:
      utils::crash(message("Unknown"));
    case tvg::Result::Success:
      break;
    default:
      utils::crash(message("Exhaustive switch error"));
  }
}

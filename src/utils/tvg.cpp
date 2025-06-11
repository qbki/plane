#include <format>

#include "src/utils/crash.h"

#include "tvg.h"

void
vg_verify_or_crash(const std::string& where, const tvg::Result& result)
{
  auto message = [&where](const std::string& value) {
    return std::format("{}: {}", where, value);
  };
  switch (result) {
    case tvg::Result::FailedAllocation:
      crash(message("FailedAllocation"));
    case tvg::Result::InsufficientCondition:
      crash(message("InsufficientCondition"));
    case tvg::Result::InvalidArguments:
      crash(message("InvalidArguments"));
    case tvg::Result::MemoryCorruption:
      crash(message("MemoryCorruption"));
    case tvg::Result::NonSupport:
      crash(message("NonSupport"));
    case tvg::Result::Unknown:
      crash(message("Unknown"));
    case tvg::Result::Success:
      break;
    default:
      crash(message("Exhaustive switch error"));
  }
}

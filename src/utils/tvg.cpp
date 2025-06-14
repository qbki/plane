#include <format>

#include "tvg.h"

import pln.utils.crash;

void
vg_verify_or_crash(const std::string& where, const tvg::Result& result)
{
  auto message = [&where](const std::string& value) {
    return std::format("{}: {}", where, value);
  };
  switch (result) {
    case tvg::Result::FailedAllocation:
      pln::utils::crash(message("FailedAllocation"));
    case tvg::Result::InsufficientCondition:
      pln::utils::crash(message("InsufficientCondition"));
    case tvg::Result::InvalidArguments:
      pln::utils::crash(message("InvalidArguments"));
    case tvg::Result::MemoryCorruption:
      pln::utils::crash(message("MemoryCorruption"));
    case tvg::Result::NonSupport:
      pln::utils::crash(message("NonSupport"));
    case tvg::Result::Unknown:
      pln::utils::crash(message("Unknown"));
    case tvg::Result::Success:
      break;
    default:
      pln::utils::crash(message("Exhaustive switch error"));
  }
}

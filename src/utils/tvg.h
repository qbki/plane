#pragma once
#include <string>
#include <thorvg.h>

void
vg_verify_or_crash(const std::string& where, const tvg::Result& result);

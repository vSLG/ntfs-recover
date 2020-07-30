// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <ctime>

namespace nf::Utils {
std::time_t time();
std::string format(const std::string &format, ...);
}
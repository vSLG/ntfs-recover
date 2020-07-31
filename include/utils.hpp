// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * Misc. helpers.
 */

#pragma once

#include <ctime>

namespace nf::Utils {
// Get current time.
std::time_t time();

// Format std::string.
std::string format(const std::string &format, ...);

// Convert bytes to human readable string.
std::string b2h(uint64_t bytes);
}
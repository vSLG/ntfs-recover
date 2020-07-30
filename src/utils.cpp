// SPDX-License-Identifier: LGPL-3.0-or-later

#include <chrono>

#include <utils.hpp>

namespace nf::Utils {
std::time_t time() {
    return std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
}
}
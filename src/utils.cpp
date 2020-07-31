// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * Misc. helpers.
 */

#include <chrono>
#include <cstdarg>
#include <cstring>
#include <math.h>
#include <string>
#include <vector>

#include <utils.hpp>

namespace nf::Utils {
std::time_t time() {
    return std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
}

std::string format(const std::string &format, ...) {
    va_list args;
    va_start(args, format);
    size_t len = std::vsnprintf(NULL, 0, format.c_str(), args);
    va_end(args);
    std::vector<char> vec(len + 1);
    va_start(args, format);
    std::vsnprintf(&vec[0], len + 1, format.c_str(), args);
    va_end(args);
    return &vec[0];
}

std::string b2h(uint64_t bytes) {
    int         idx     = floor(log(bytes) / log(1024));
    std::string sizes[] = {"", "KiB", "MiB", "GiB", "TiB"};

    idx = idx > 5 ? 5 : idx;

    if (idx == 0)
        return std::to_string(bytes) + " B";
    return format("%.2f %s", bytes / pow(1024, idx), sizes[idx].c_str());
}
} // namespace nf::Utils
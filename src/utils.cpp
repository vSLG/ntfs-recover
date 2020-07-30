// SPDX-License-Identifier: LGPL-3.0-or-later

#include <chrono>
#include <cstdarg>
#include <cstring>
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
} // namespace nf::Utils
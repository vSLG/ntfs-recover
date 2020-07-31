// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * UNIX IO API.
 */

#pragma once

#include <filesystem>

#include <io/base.hpp>

namespace nf {
class IO : IOBase {
  public:
    static FILE *open(const std::string &path, OpenMode mode = OpenMode::READ);
    static bool  is_mounted(const std::string &path);
    static std::filesystem::path read_link(const std::string &path);
};
} // namespace nf
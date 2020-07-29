// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <io/base.hpp>

namespace nf {
class IO : IOBase {
  public:
    static std::fstream *open(std::string path, OpenMode mode = OpenMode::READ);
    static bool          is_mounted(std::string path);
};
} // namespace nf
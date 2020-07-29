// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <io/base.hpp>

namespace nf {
class IO : IOBase {
  public:
    std::fstream open(std::string path, OpenMode mode = OpenMode::READ);
};
} // namespace nf
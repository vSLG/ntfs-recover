// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <fstream>
#include <string>

namespace nf {
class IOBase {
  public:
    enum OpenMode { READ = 1, WRITE };

    static FILE *open(std::string path, OpenMode mode = OpenMode::READ);
    static bool  is_mounted(std::string path);
};
}; // namespace nf
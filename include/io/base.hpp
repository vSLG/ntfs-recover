// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <fstream>
#include <string>

namespace nf {
class IOBase {
  public:
    enum OpenMode { READ = 1, WRITE };

    // XXX: Not good to use fstream on Windows
    virtual std::fstream open(std::string path,
                              OpenMode    mode = OpenMode::READ) = 0;
};
}; // namespace nf
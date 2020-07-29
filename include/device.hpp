// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <io.hpp>

namespace nf {
class NTFSDevice {
  private:
    std::fstream _dev;

  public:
    NTFSDevice(std::string dev_path);
    ~NTFSDevice();

    int open(IOBase::OpenMode mode = IOBase::OpenMode::READ);
    int close();
};
} // namespace nf
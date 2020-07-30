// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <io.hpp>

namespace nf {
class NTFSDevice {
  private:
    FILE *      _dev = nullptr;
    std::string _dev_path;
    uint16_t    _sector_size = 512; // Assume it is 512 until we parse header

  public:
    NTFSDevice(std::string dev_path);
    ~NTFSDevice();

    int   open(IOBase::OpenMode mode = IOBase::OpenMode::READ);
    int   close();
    char *read(uint32_t size);
    char *read_sectors(uint32_t quant = 1);
    int   load_header();

    // Getters and setters
    std::string dev_path();
    uint16_t    sector_size();
};
} // namespace nf
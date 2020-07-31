// SPDX-License-Identifier: LGPL-3.0-or-later

// Temporary main program for testing purpose

#include <iostream>

#include <log.hpp>
#include <ntfs/device.hpp>

using namespace nf;

int main(int argc, char *argv[]) {
    Log::log::reporting_level() = Log::DEBUG;

    std::string device = argc >= 2 ? argv[1] : "./test.img";

    NTFS::Device *dev = new NTFS::Device(device);

    if (dev->open())
        return 1;

    if (dev->load_header())
        return 1;

    dev->dump_part_bootsector();

    return 0;
}
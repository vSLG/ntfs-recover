// SPDX-License-Identifier: LGPL-3.0-or-later

// Temporary main program for testing purpose

#include <iostream>

#include <log.hpp>
#include <ntfs/device.hpp>

using namespace nf;

int main(int argc, char *argv[]) {
    Log::log::reporting_level() = Log::DEBUG;

    I << "size_t: " << std::numeric_limits<std::streamsize>::max();
    std::string device = argc >= 2 ? argv[1] : "./test.img";

    NTFSDevice *dev = new NTFSDevice(device);
    if (dev->open())
        return 1;

    if (dev->load_header())
        return 1;

    return 0;
}
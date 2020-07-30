// SPDX-License-Identifier: LGPL-3.0-or-later

// Temporary main program for testing purpose

#include <iostream>

#include <device.hpp>
#include <log.hpp>

using namespace nf;

int main(int argc, char *argv[]) {
    std::cout << "kk eae men\n";

    Log::log::reporting_level() = Log::DEBUG;

    NTFSDevice *dev = new NTFSDevice("/dev/sda2");
    if (dev->open())
        return 1;

    return 0;
}
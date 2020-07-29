// SPDX-License-Identifier: LGPL-3.0-or-later

// Temporary main program for testing purpose

#include <iostream>

#include <device.hpp>

using namespace nf;

int main(int argc, char *argv[]) {
    std::cout << "kk eae men\n";

    NTFSDevice *dev = new NTFSDevice("/dev/sda2");
    dev->open();

    return 0;
}
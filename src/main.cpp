// SPDX-License-Identifier: LGPL-3.0-or-later

// Temporary main program for testing purpose

#include <iostream>

#include <device.hpp>
#include <log.hpp>

using namespace nf;

int main(int argc, char *argv[]) {
    std::cout << "kk eae men\n";

    Log::log::reporting_level() = Log::DEBUG;

    D << "kk eae debug";
    I << "kk eae info";
    W << "kk eae warn";
    E << "kk eae err";
    C << "kk eae crit";

    NTFSDevice *dev = new NTFSDevice("/dev/sda2");
    dev->open();

    return 0;
}
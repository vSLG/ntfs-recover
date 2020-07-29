// SPDX-License-Identifier: LGPL-3.0-or-later

#include <device.hpp>
#include <io.hpp>

using namespace nf;

NTFSDevice::NTFSDevice(std::string dev_path) {
    this->_dev_path = dev_path;
}

NTFSDevice::~NTFSDevice() {
    this->close();
}

int NTFSDevice::open(IOBase::OpenMode mode) {
    if (this->_dev)
        return -1;

    this->_dev = IO::open(this->_dev_path, mode);
    return 0;
}

int NTFSDevice::close() {
    this->_dev->close();
    return 0;
}

// Getters and setters

std::string NTFSDevice::dev_path() {
    return this->_dev_path;
}
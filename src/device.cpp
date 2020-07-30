// SPDX-License-Identifier: LGPL-3.0-or-later

#include <device.hpp>
#include <io.hpp>
#include <log.hpp>

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

    D << "Trying to open " << this->_dev_path << "...";
    this->_dev = IO::open(this->_dev_path, mode);

    if (!this->_dev)
        return 1;

    D << this->_dev_path << " opened successfully";
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
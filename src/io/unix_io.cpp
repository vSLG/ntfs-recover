// SPDX-License-Identifier: LGPL-3.0-or-later

#include <exceptions.hpp>
#include <io/unix_io.hpp>

using namespace nf;

std::fstream IO::open(std::string path, OpenMode mode) {
    std::fstream            dev;
    std::ios_base::openmode flags;

    if (mode & OpenMode::READ)
        flags |= std::ios::in;
    if (mode & OpenMode::WRITE)
        flags |= std::ios::out;

    flags |= std::ios::binary;
    dev.open(path, flags);

    if (dev.bad() || !dev.is_open())
        throw exception::OpenException(path);

    return dev;
}
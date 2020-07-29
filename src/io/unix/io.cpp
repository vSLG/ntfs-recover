// SPDX-License-Identifier: LGPL-3.0-or-later

#include <mntent.h>

#include <exceptions.hpp>
#include <io/unix_io.hpp>

using namespace nf;

std::fstream *IO::open(std::string path, OpenMode mode) {
    std::fstream *          dev;
    std::ios_base::openmode flags;

    if (is_mounted(path))
        throw exception::OpenException(path,
                                       "device is mounted, refusing to open");

    if (mode & OpenMode::READ)
        flags |= std::ios::in;
    if (mode & OpenMode::WRITE)
        flags |= std::ios::out;

    flags |= std::ios::binary;
    dev->open(path, flags);

    if (dev->fail() || !dev->is_open())
        throw exception::OpenException(path);

    return dev;
}

bool IO::is_mounted(std::string path) {
    // XXX: Not portable to always use /etc/mtab
    FILE *         mtab = setmntent("/etc/mtab", "r");
    struct mntent *mnt;

    while ((mnt = getmntent(mtab)))
        if (mnt->mnt_fsname == path)
            return true;

    return false;
}
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <filesystem>
#include <mntent.h>

#include <exceptions.hpp>
#include <io/unix_io.hpp>
#include <log.hpp>

using namespace nf;

FILE *IO::open(std::string path, OpenMode mode) {
    FILE *      dev   = nullptr;
    std::string flags = "";

    try {
        if (!std::filesystem::exists(path))
            throw exception::FileNotFoundException(path);

        if (IO::is_mounted(path))
            throw exception::OpenException(
                path, "device is mounted, refusing to open");

        if (mode & OpenMode::READ)
            flags += "r";
        if (mode & OpenMode::WRITE)
            flags += "w";

        flags += "b";

        dev = fopen64(path.c_str(), flags.c_str());

        if (!dev)
            throw exception::OpenException(path, "permission denied");
    } catch (const exception::NTFSException &e) {
        C << e.what();
        return nullptr;
    }

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
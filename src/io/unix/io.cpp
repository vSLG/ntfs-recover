// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * Implementation for IO operations on an UNIX system.
 */

#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <mntent.h>

#include <exceptions.hpp>
#include <io/unix_io.hpp>
#include <log.hpp>

namespace fs = std::filesystem;
using namespace nf;

FILE *IO::open(const std::string &path, OpenMode mode) {
    FILE *      dev   = nullptr;
    std::string flags = "";

    try {
        if (!fs::exists(path))
            throw exception::FileNotFoundException(path);

        if (!fs::is_block_file(IO::read_link(path))) {
            W << "Specified device is not a block device.";
        }

        if (IO::is_mounted(path))
            throw exception::IO::OpenException(
                path, "device is mounted, refusing to open");

        // Acquire exclusive lock on device.
        flock flk;
        std::memset(&flk, 0, sizeof(flk));

        if (mode & OpenMode::READ) {
            flags += "r";
            flk.l_type = F_RDLCK;
        }
        if (mode & OpenMode::WRITE) {
            flags += "w";
            flk.l_type = F_WRLCK;
        }

        flags += "b";
        dev = fopen64(path.c_str(), flags.c_str());

        if (!dev)
            throw exception::IO::OpenException(path);

        flk.l_whence = SEEK_SET;
        flk.l_start = flk.l_len = 0ll;

        int ret;

        if ((ret = fcntl(fileno(dev), F_SETLK, &flk))) {
            fclose(dev);
            free(dev);
            throw exception::IO::OpenException(path);
        }
    } catch (const exception::NTFSException &e) {
        C << e.what();
        return nullptr;
    }

    return dev;
}

bool IO::is_mounted(const std::string &path) {
    // XXX: Not portable to always use /etc/mtab
    FILE *         mtab = setmntent("/etc/mtab", "r");
    struct mntent *mnt;

    while ((mnt = getmntent(mtab))) {
        fs::path    real_path = IO::read_link(mnt->mnt_fsname);
        std::string name(real_path.stem());

        // If it is a loop device, check for its backing file
        if ((name.find("loop") != std::string::npos) &&
            (path.find("/dev/loop") == std::string::npos)) {
            std::ifstream backing_file("/sys/block/" + name +
                                       "/loop/backing_file");

            backing_file >> real_path;
            backing_file.close();
        }

        if (real_path == IO::read_link(path))
            return true;
    }

    return false;
}

fs::path IO::read_link(const std::string &path) {
    fs::path fspath = fs::absolute(path).lexically_normal();

    if (!fs::is_symlink(fspath))
        return fspath;

    fs::path real = fs::read_symlink(fspath);

    if (real.is_relative())
        real = fspath.parent_path() / real;

    return real.lexically_normal();
}
// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * Program defined exceptions.
 */

#pragma once

#include <stdexcept>
#include <string>

namespace nf::exception {
class NTFSException : public std::runtime_error {
  public:
    explicit NTFSException(const std::string &what,
                           const std::string &prefix = "NTFS Error")
        : std::runtime_error(prefix + ": " + what){};
};

class FileNotFoundException : public NTFSException {
  public:
    explicit FileNotFoundException(std::string path)
        : NTFSException("file \"" + path + "\" does not exist"){};
};

namespace IO {
class IOException : public NTFSException {
  public:
    explicit IOException(const std::string &what)
        : NTFSException(what, "IO Error"){};
};

class OpenException : public IOException {
  public:
    explicit OpenException(std::string dev_path, std::string reason = "")
        : IOException(("could not open device " + dev_path + ". Reason: ") +
                      (reason != "" ? reason : strerror(errno))){};
};

class ReadException : public IOException {
  public:
    explicit ReadException(uint16_t sector)
        : IOException("could not read sector " + sector){};
};
} // namespace IO
}; // namespace nf::exception
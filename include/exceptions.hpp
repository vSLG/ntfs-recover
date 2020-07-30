// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <stdexcept>
#include <string>

namespace nf {
namespace exception {
class NTFSException : public std::runtime_error {
  public:
    explicit NTFSException(std::string what)
        : std::runtime_error("NTFS Error: " + what){};
};

class OpenException : public NTFSException {
  public:
    explicit OpenException(std::string dev_path, std::string reason = "")
        : NTFSException(("could not open device " + dev_path) +
                        (reason != "" ? ". Reason: " + reason : "")){};
};

class FileNotFoundException : public NTFSException {
  public:
    explicit FileNotFoundException(std::string path)
        : NTFSException("File \"" + path + "\" does not exist"){};
};
}; // namespace exception
}; // namespace nf
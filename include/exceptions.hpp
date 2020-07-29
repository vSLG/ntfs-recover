// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <stdexcept>
#include <string>

namespace nf {
namespace exception {
class NTFSException : public std::runtime_error {
  public:
    explicit NTFSException(std::string what)
        : std::runtime_error("NTFS Error: " + what) {
    }
};

class OpenException : public NTFSException {
  public:
    OpenException(std::string dev_path, std::string reason = "")
        : NTFSException(("could not open device " + dev_path) +
                        (reason != "" ? ". Reason: " + reason : "")){};
};
}; // namespace exception
}; // namespace nf
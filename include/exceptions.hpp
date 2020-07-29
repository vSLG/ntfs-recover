// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <exception>
#include <string>

namespace nf {
namespace exception {
class NTFSException : public std::exception {
  private:
    const char *_what;

  public:
    NTFSException(std::string what) {
        what        = "NTFS Error: " + what;
        this->_what = what.c_str();
    }

    const char *what() const throw() {
        return this->_what;
    }
};

class OpenException : public NTFSException {
  public:
    OpenException(std::string dev_path, std::string reason = "")
        : NTFSException("could not open device " + dev_path + reason != ""
                            ? ". Reason: " + reason
                            : ""){};
};
}; // namespace exception
}; // namespace nf
// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * MTF related classes implementation
 */

#include <ntfs/mtf.hpp>

using namespace nf::NTFS;

MTFEntry::MTFEntry(const char *data, Device *dev) : _dev(dev) {
}

MTFAttribute::MTFAttribute(const char *data, Device *dev) : _dev(dev) {
}

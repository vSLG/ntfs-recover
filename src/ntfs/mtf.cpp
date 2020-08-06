// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * MTF related classes implementation
 */

#include <exceptions.hpp>
#include <log.hpp>
#include <ntfs/mtf.hpp>

using namespace nf::NTFS;

namespace err = nf::exception::MTF;

MTFEntry::MTFEntry(char *data, Device *dev) : _dev(dev) {
    mtf_header_t *header = (mtf_header_t *) data;

    // First check if this MTF entry is healthy
    uint16_t fixup        = data[header->uso];
    uint16_t actual_value = data[header->uso + 2];

    for (int i = 1; i < header->usc; i++)
        if ((uint16_t) data[dev->sector_size() * i - 2] == fixup)
            data[dev->sector_size() * i - 2] = (char) actual_value;
        else
            throw err::InvalidMTFException(header->rec_number);

    if (!(header->flags & ENTRY_MASK))
        throw err::InvalidMTFException(header->rec_number);

    if (!header->bytes_used || header->bytes_alloc != dev->mtf_size() ||
        header->bytes_used > dev->mtf_size())
        throw err::InvalidMTFException(header->rec_number);

    // uint16_t current_offset = header->attr_off;

    // while ((uint32_t) data[current_offset] != 0xffffffff) {}

    this->_header = header;
    this->_data   = data;
    D << "MTF entry OK";
}

MTFAttribute::MTFAttribute(const char *data, Device *dev) : _dev(dev) {
}

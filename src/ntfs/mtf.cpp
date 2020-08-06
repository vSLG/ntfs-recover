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

    uint16_t current_offset = header->attr_off;

    while ((uint32_t) data[current_offset] != ATTR_END) {
        MTFAttribute *attr = new MTFAttribute(data + current_offset, dev);
        current_offset += attr->size();
        this->_attrs.push_back(attr);

        if (current_offset > dev->mtf_size())
            throw err::InvalidMTFException(header->rec_number);
    }

    this->_header = header;
    this->_data   = data;
    D << "MTF entry OK";
}

MTFAttribute::MTFAttribute(char *data, Device *dev) : _dev(dev) {
    mtf_attr_header_t *header = (mtf_attr_header_t *) data;

    // Is this attribute type valid?
    if ((header->kind == 0) || !(header->kind & ATTR_MASK) ||
        ((header->kind % 0x10 != 0) && (header->kind != ATTR_END)))
        throw err::InvalidMTFAttrException(header->kind);

    // Is non_resident set to anything else than 0 or 1?
    if (header->non_resident & 0xfe)
        throw err::InvalidMTFAttrException(header->kind);

    // Are the flags valid?
    if (!(header->flags & ATTR_FLAG_MASK) && (header->flags != 0))
        throw err::InvalidMTFAttrException(header->kind);

    // Is the size right?
    if (header->len == 0 ||
        header->len > dev->mtf_size() - sizeof(mtf_attr_header_t))
        throw err::InvalidMTFAttrException(header->kind);

    // Check resident fields
    if (!(header->non_resident)) {
        // Is content within bounds?
        if (header->data.resident.content_len +
                header->data.resident.content_offset >
            header->len)
            throw err::InvalidMTFAttrException(header->kind);
    } /* Check non-resident fields */ else {
        // TODO
    }

    this->_header = header;
}

uint32_t MTFAttribute::size() {
    return this->_header->len;
}

mtf_attr_kind_t MTFAttribute::type() {
    return this->_header->kind;
}
// SPDX-License-Identifier: LGPL-3.0-or-later

// Temporary main program for testing purpose

#include <iostream>

#include <log.hpp>
#include <ntfs/device.hpp>
#include <ntfs/mtf.hpp>

using namespace nf;

int main(int argc, char *argv[]) {
    Log::log::reporting_level() = Log::DEBUG;

    std::string device = argc >= 2 ? argv[1] : "./test.img";

    NTFS::Device *dev = new NTFS::Device(device);

    if (dev->open())
        return 1;

    if (dev->load_header())
        return 1;

    dev->dump_part_bootsector();

    char *        data   = dev->read_sectors(2, 4 * 8);
    mtf_header_t *header = (mtf_header_t *) data;
    MTFEntry(data, dev);

    I << "Magic:        " << header->magic;
    I << "Fixup offset: " << header->uso;
    I << "Fixup count:  " << header->usc;

    I << "Offset to first attribute:      " << header->attr_off;
    I << Utils::format("Flags:                          0x%.2x", header->flags);
    I << "Used bytes for this entry:      " << header->bytes_used;
    I << "Bytes allocated for this entry: " << header->bytes_alloc;
    I << "Entry id: " << Utils::format("%.32x", header->rec_number);

    return 0;
}
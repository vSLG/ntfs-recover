// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * Implementation of main operations of device.
 */

#include <io.hpp>
#include <log.hpp>
#include <ntfs/bootsector.hpp>
#include <ntfs/device.hpp>

using namespace nf::NTFS;

Device::Device(std::string dev_path) {
    this->_dev_path = dev_path;
}

Device::~Device() {
    this->close();
}

int Device::open(IOBase::OpenMode mode) {
    if (this->_dev)
        return -1;

    this->_dev = IO::open(this->_dev_path, mode);

    if (!this->_dev)
        return 1;

    D << this->_dev_path << " opened successfully";
    return 0;
}

int Device::close() {
    // TODO: return
    fclose(this->_dev);
    free(this->_dev);
    return 0;
}

/*
 * Read `size` bytes from disk.
 */
char *Device::read(uint32_t size) {
    if (!this->_dev)
        return nullptr;

    char *buf = (char *) malloc(size);

    fread(buf, size, 1, this->_dev);
    return buf;
}

/*
 * Read `quant` sectors from disk.
 */
char *Device::read_sectors(uint32_t quant) {
    return this->read(quant * this->nbs->bpb.bps);
}

/*
 * Read first sector and parse partition information.
 */
int Device::load_header() {
    nbs_t *nbs = nullptr;

    // Return pointer to begin just in case
    fseeko64(this->_dev, 0, SEEK_SET);

    if (!(nbs = (nbs_t *) this->read_sectors(1))) {
        E << "Could not read first sector of the partition, unable to load "
             "partition info.";
        return -1;
    }

    D << "Reported partition OEM: '" << nbs->oem << "'";

    int warnings = 0;
    warnings += this->check_signature(nbs);
    warnings += this->check_sector_size(nbs);
    warnings += this->check_partition_size(nbs);
    warnings += this->check_mtf_cluster(nbs);

    if (warnings) {
        I << "Generated " << warnings << " warnings during boot sector check.";
    } else {
        I << "Boot sector check OK.";
    }

    return warnings;
}

void Device::dump_part_bootsector(FILE *stream) {
    fprintf(stream, "Partition: %s\n", this->_dev_path.c_str());
    fprintf(stream,
            "\tPartition size:\t %s (%lu bytes)\n",
            Utils::b2h(this->bpb->bps * this->bpb->ts).c_str(),
            this->bpb->bps * this->bpb->ts);
    fprintf(
        stream, "\tSector size:\t %s\n", Utils::b2h(this->bpb->bps).c_str());
    fprintf(stream,
            "\tCluster size:\t %s\n",
            Utils::b2h(this->bpb->bps * this->bpb->spc).c_str());
    fprintf(
        stream, "\tTotal clusters:\t %lu\n", this->bpb->ts / this->bpb->spc);
    fprintf(stream, "\t$MTF cluster:\t 0x%.8lx\n", this->ntfs_header->mtf);
    fprintf(stream, "\t$MTFmir cluster: 0x%.8lx\n", this->ntfs_header->mtf2);
}

// Getters and setters

std::string Device::dev_path() {
    return this->_dev_path;
}

uint16_t Device::sector_size() {
    return this->nbs->bpb.bps;
}
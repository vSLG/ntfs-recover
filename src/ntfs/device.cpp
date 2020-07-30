// SPDX-License-Identifier: LGPL-3.0-or-later

#include <io.hpp>
#include <log.hpp>
#include <ntfs/device.hpp>
#include <ntfs/layout.hpp>

using namespace nf;

NTFSDevice::NTFSDevice(std::string dev_path) {
    this->_dev_path = dev_path;
}

NTFSDevice::~NTFSDevice() {
    this->close();
}

int NTFSDevice::open(IOBase::OpenMode mode) {
    if (this->_dev)
        return -1;

    this->_dev = IO::open(this->_dev_path, mode);

    if (!this->_dev)
        return 1;

    D << this->_dev_path << " opened successfully";
    return 0;
}

int NTFSDevice::close() {
    fclose(this->_dev);
    return 0;
}

/*
 * Read `size` bytes from disk.
 */
char *NTFSDevice::read(uint32_t size) {
    if (!this->_dev)
        return nullptr;

    char *buf = (char *) malloc(size);

    fread(buf, size, 1, this->_dev);
    return buf;
}

/*
 * Read `quant` sectors from disk.
 */
char *NTFSDevice::read_sectors(uint32_t quant) {
    return this->read(quant * this->_sector_size);
}

/*
 * Read first sector and parse partition information.
 */
int NTFSDevice::load_header() {
    NTFS::nbs_t *nbs = nullptr;

    // Return pointer to begin just in case
    fseeko64(this->_dev, 0, SEEK_SET);

    if (!(nbs = (NTFS::nbs_t *) this->read_sectors(1))) {
        E << "Could not read first sector of the partition, unable to load "
             "partition info.";
        return -1;
    }

    // ------------------------------------------------------------------------
    // Check if partition has valid NTFS signature.
    if (NTFS::NTFSMagic != (char *) nbs->oem) {
        W << "This partition does not seem to be NTFS or it is corrupt.";
        W << "Partition registered OEM: " << nbs->oem;
    }

    // ------------------------------------------------------------------------
    // Check if reported bytes per sector is valid
    if ((nbs->bpb.bps != 0) && ((nbs->bpb.bps & (nbs->bpb.bps - 1)) == 0)) {
        this->_sector_size = nbs->bpb.bps;
    } else {
        W << "Wrong sector size reported, guessing 512.";
    }

    // ------------------------------------------------------------------------
    // Check if reported partition size is beyound our seek capability.
    // This may sign NBS is corrupt or outdated even if FS sign is valid.
    size_t current = ftello64(this->_dev);
    fseeko64(this->_dev, 0, SEEK_END);
    size_t bytes_detected = ftello64(this->_dev) - 512l;
    fseeko64(this->_dev, current, SEEK_SET);

    if (bytes_detected < this->_sector_size * nbs->bpb.ts) {
        W << "Partition size may be wrong. Read size: "
          << this->_sector_size * nbs->bpb.ts
          << " bytes; detected size: " << bytes_detected << " bytes.";
    }

    // ------------------------------------------------------------------------
    // Check if MTF cluster is within seek range
    if (nbs->ntfs_header.mtf >
        bytes_detected * this->_sector_size * nbs->bpb.spc) {
        W << "Reported $MTF cluster outside read bounds.";
    }

    if (nbs->ntfs_header.mtf2 >
        bytes_detected * this->_sector_size * nbs->bpb.spc) {
        W << "Reported $MTFmir cluster outside read bounds.";
    }

    D << "Partition information:";
    D << "\tBytes per sector: " << this->_sector_size;
    D << "\tTotal sectors: " << nbs->bpb.ts;
    D << "\tApparent size: " << nbs->bpb.bps * nbs->bpb.ts / 1024.F / 1024.F
      << " MiB";
    D << "\t----------------------------------";
    D << "\tMTF cluster: " << nbs->ntfs_header.mtf;
    D << "\tMTFmir cluster: " << nbs->ntfs_header.mtf2;

    return 0;
}

// Getters and setters

std::string NTFSDevice::dev_path() {
    return this->_dev_path;
}

uint16_t NTFSDevice::sector_size() {
    return this->_sector_size;
}
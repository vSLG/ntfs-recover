// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <io.hpp>
#include <ntfs/bootsector.hpp>

namespace nf::NTFS {
class Device {
  private:
    FILE *         _dev = nullptr;
    std::string    _dev_path;
    nbs_t *        nbs         = new nbs_t();
    bpb_t *        bpb         = &(nbs->bpb);
    ntfs_header_t *ntfs_header = &(nbs->ntfs_header);

  public:
    Device(std::string dev_path);
    ~Device();

    int   open(IOBase::OpenMode mode = IOBase::OpenMode::READ);
    int   close();
    char *read(uint32_t size);
    char *read_sectors(uint32_t quant = 1);
    int   load_header();

    void dump_part_bootsector(FILE *stream = stdout);

    // ------------------------------------------------------------------------
    // Device checking

    // Check if partition has valid NTFS signature.
    int check_signature(nbs_t *nbs);

    // Check if reported bytes per sector is valid
    int check_sector_size(nbs_t *nbs);

    // Check if reported partition size is beyound our seek capability.
    // This may sign NBS is corrupt or outdated even if FS sign is valid.
    int check_partition_size(nbs_t *nbs);

    // Check whether MTF cluster is within seek range and it is valid.
    int check_mtf_cluster(nbs_t *nbs);

    // ------------------------------------------------------------------------
    // Getters and setters
    std::string dev_path();
    uint16_t    sector_size();
};
} // namespace nf::NTFS
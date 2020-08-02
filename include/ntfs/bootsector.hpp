// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * NTFS boot sector related structs and classes.
 */

#pragma once

#include <types.hpp>

namespace nf::NTFS {
#define NTFSMagic "NTFS    "

/*
 * BIOS parameter block (bpb) structure.
 */
typedef struct __attribute__((__packed__)) bpb {
    uint16_t bps = 512; // bytes per sector
    char     spc = 4;   // sectors per cluster
    uint16_t rs;        // reserved sectors, must be 0
    char     tc;        // table count
    uint16_t rec;       // root entry count
    uint16_t sc;        // sector count
    char     md;        // media descriptor. F8 = hard disk
    uint16_t sptb;      // sectors per table
    uint16_t sptr;      // sectors per track
    uint16_t hds;       // heads
    uint32_t hsc;       // hidden sector count
    uint32_t ts32;      // total sectors (32 bit)
    uint32_t rv1;       // reserved, must be 0
    uint64_t ts;        // total sectors
} bpb_t;
static_assert(sizeof(bpb_t) == 37);

/*
 * NTFS Header structure.
 */
typedef struct __attribute__((__packed__)) ntfs_header {
    uint64_t mtf  = 0;    // cluster for $MTF
    uint64_t mtf2 = 0;    // cluster for $MTFmir
    char     cpr  = 0xf6; // clusters per record, 0xf6 on most cases
    uint8_t  nu2[3];      // not used
    char     cpib = 0;    // clusters per index buffer
    uint8_t  nu3[3];      // not used
    uint64_t sn;          // serial number
    uint32_t nu4;         // not used
} ntfs_header_t;
static_assert(sizeof(ntfs_header_t) == 36);

/*
 * NTFS Boot Sector (nbs) structure.
 */
typedef struct __attribute__((__packed__)) nbs {
    uint8_t       jmp[3];      // jump instruction
    uint8_t       oem[8];      // OEM ID
    bpb_t         bpb;         // BIOS parameter block
    ntfs_header_t ntfs_header; // NTFS header
    uint8_t       bc[426];     // bootstrap code
    uint8_t       esm[2];      // end sector mark
} nbs_t;
static_assert(sizeof(nbs_t) == 512);
} // namespace nf::NTFS

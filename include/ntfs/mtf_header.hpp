// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * MTF header related structs.
 */

#pragma once

#include <types.hpp>

namespace nf::NTFS {
/*
 * MTF attribute kind.
 */
typedef enum mtf_attr_kind {
    ATTR_STD_INFO        = 0x10,       // $STANDARD_INFORMATION
    ATTR_LIST            = 0x20,       // $ATTRIBUTE_LIST
    ATTR_FILE_NAME       = 0x30,       // $FILE_NAME
    ATTR_OBJ_ID          = 0x40,       // $OBJECT_ID
    ATTR_SEC_DESCRIPTOR  = 0x50,       // $SECURITY_DESCRIPTOR
    ATTR_VOL_NAME        = 0x60,       // $VOLUME_NAME
    ATTR_VOL_INFO        = 0x70,       // $VOLUME_INFORMATION
    ATTR_DATA            = 0x80,       // $DATA
    ATTR_INDEX_ROOT      = 0x90,       // $INDEX_ROOT
    ATTR_INDEX_ALLOC     = 0xa0,       // $INDEX_ALLOCATION
    ATTR_BITMAP          = 0xb0,       // $BITMAP
    ATTR_REPARSE_POINT   = 0xc0,       // $REPARSE_POINT
    ATTR_EA_INFO         = 0xd0,       // $EA_INFORMATION
    ATTR_EA              = 0xe0,       // $EA
    ATTR_PROP_SET        = 0xf0,       // $PROPERTY_SET
    ATTR_LOG_UTIL_STREAM = 0x100,      // $LOGGED_UTILITY_STREAM
    ATTR_MASK            = 0x1f0,      // Mask valid values
    ATTR_END             = 0xffffffff, // End mark of MTF attributes.
} mtf_attr_kind_t;

/*
 * MTF attribute flag (2 bytes)
 */
typedef enum __attribute__((__packed__)) mtf_attr_flag {
    ATTR_COMPRESSED = 0x0001, // Attribute is compressed
    ATTR_ENCRYPTED  = 0x4000, // Attribute is encrypted
    ATTR_SPARSE     = 0x8000, // Attribute is sparse
    ATTR_FLAG_MASK  = 0xc001,
} mtf_attr_flag_t;

/*
 * MTF entry flag
 */
typedef enum __attribute__((__packed__)) mtf_entry_flag {
    ENTRY_IN_USE    = 0x01,
    ENTRY_DIRECTORY = 0x02,
    ENTRY_MASK      = 0x03,
} mtf_entry_flag_t;

/*
 * MTF record header.
 */
typedef struct __attribute__((__packed__)) mtf_header {
    char     magic[4];      // magic number "FILE"      0
    uint16_t uso;           // update sequence offset   4
    uint16_t usc;           // update sequence count    6
    uint64_t lsn;           // log file sequence number 8
    uint16_t rsn;           // record sequence number   16
    uint16_t hlc;           // hard link count          18
    uint16_t attr_off;      // attributes offset        20
    uint16_t flags;         // flags                    22
    uint32_t bytes_used;    // bytes in use             24
    uint32_t bytes_alloc;   // bytes allocated          28
    uint64_t parent_rec;    // parent record            32
    uint32_t next_attr_idx; // next attribute index     40
    uint32_t rv1;           // reserved                 44
    uint64_t rec_number;    // record number            48
} mtf_header_t;
static_assert(sizeof(mtf_header_t) == 56);

/*
 * MTF attribute header.
 */
typedef struct __attribute__((__packed__)) mtf_attr_header {
    mtf_attr_kind_t kind;  // Kind of attribute
    uint32_t        len;   // Length of attribute. Determines the next attribute
                           // location
    uint8_t  non_resident; // Is set if attribute content is outside MTF record
    uint8_t  name_len;     // Length of the name
    uint16_t name_offset;  // Offset to name, if name_len != 0. Name data is
                           // stored in unicode
    mtf_attr_flag_t flags; // Tells if data is compressed, encrypted or sparse
    uint16_t        id;    // Attribute ID. It must be unique in this MTF record

    union __attribute__((__packed__)) {
        // Resident attribute
        struct __attribute__((__packed__)) {
            uint32_t content_len;    // Length of content
            uint16_t content_offset; // Offset to content
        } resident;
        static_assert(sizeof(resident) == 6);

        // Non resident attribute
        struct __attribute__((__packed__)) {
            int64_t low_vcn;   // Lowest virtual cluster number used. Only has a
                               // value if attribute type is $ATTRIBUTE_LIST
            int64_t  high_vcn; // Highest virtual cluster number used
            uint16_t runlist_offset;   // Offset to runlist
            uint8_t  compression_unit; // Used compression unit size
            uint8_t  unused[5];
            uint64_t alloc_bytes;   // Allocated size in disk
            uint64_t content_bytes; // Actual size of the content, regardless if
                                    // it's compressed or not
            uint64_t initialized_bytes; // Size of the initialized content
            uint64_t compressed_bytes;  // Size of content after compression
        } non_resident;
        static_assert(sizeof(non_resident) == 56);
    } data;
} mtf_attr_header_t;
static_assert(sizeof(mtf_attr_header_t) == 72);
} // namespace nf::NTFS
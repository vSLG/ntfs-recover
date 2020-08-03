// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * MTF attributes related structs.
 */

#pragma once

#include <types.hpp>

#include <ntfs/mtf_header.hpp>

namespace nf::NTFS {
/*
 * Common structs
 */

// File attributes
typedef enum file_attr {
    FILE_ATTR_READONLY    = 0x0001,
    FILE_ATTR_HIDDEN      = 0x0002,
    FILE_ATTR_SYSTEM      = 0x0004,
    FILE_ATTR_ARCHIVE     = 0x0020,
    FILE_ATTR_DEVICE      = 0x0040,
    FILE_ATTR_NORMAL      = 0x0080,
    FILE_ATTR_TEMP        = 0x0100,
    FILE_ATTR_SPARSE      = 0x0200,
    FILE_ATTR_REPARSE     = 0x0400,
    FILE_ATTR_COMPRESSED  = 0x0800,
    FILE_ATTR_OFFLINE     = 0x1000,
    FILE_ATTR_NOT_INDEXED = 0x2000,
    FILE_ATTR_ENCRYPTED   = 0x4000,
} file_attr_t;

/*
 * Time struct used by $STANDARD_INFORMATION and $FILE_NAME
 * NOTE: Time values are stored in 100 ns since Jan 1, 1600
 * NOTE: These time values are also present on $FILE_NAME, but only fields from
 *       $STANDARD_INFO are updated when a file has been modified/acessed.
 */
typedef struct __attribute__((__packed__)) time_values {
    uint64_t creation_time;    // Time created
    uint64_t content_mod_time; // Last time data attribute was modified
    uint64_t mtf_mod_time;     // Last time this mtf was modified
    uint64_t access_time;      // Last time when this file has been accessed
} time_values_t;
static_assert(sizeof(time_values_t) == 32);

/*
 * Attribute: $STANDARD_INFORMATION (0x10)
 * NOTE: ALWAYS resident
 * NOTE: Present in all file records
 */

// $STANDARD_INFO struct. Only NTFS v3.0+ supported.
typedef struct __attribute__((__packed__)) mtf_std_info {
    time_values_t time;          // Common time values
    file_attr_t   flags;         // File flags/permissions
    uint32_t      max_ver;       // Maximum number of versions
    uint32_t      version;       // Current version (if any)
    uint32_t      class_id;      // Class ID
    uint32_t      owner_id;      // Who owns this file, if quota enabled
    uint32_t      security_id;   // Security ID
    uint64_t      quota_charged; // Max file bytes, if quota enabled
    uint64_t      usn; // Last update sequence number, if journal is enabled
} mtf_std_info_t;
static_assert(sizeof(mtf_std_info_t) == 72);

/*
 * Attribute: $FILE_NAME (0x30)
 * NOTE: Resident or non-resident
 * NOTE: Content consists of a sequence of variable length of attr_list_entry_t
 * NOTE: Has no end mark, we need to keep track using the length
 */
typedef struct __attribute__((__packed__)) attr_list_entry {
    mtf_attr_kind_t type;        // Type of attribute referenced
    uint16_t        bytes;       // Byte size of this entry
    uint8_t         name_len;    // Length of the name in UTF-16 chars
    uint8_t         name_offset; // Offset to attribute name
    uint64_t        low_vcn;     // Lowest virtual cluster number for this entry
    uint64_t mtf_ref; // Reference to the MTF entry holding this attribute
    uint16_t id;      // ID of referenced attribute
    wchar_t  name[];  // Used only when creating. When reading, use name_offset
} attr_list_entry_t;

/*
 * Attribute: $FILE_NAME (0x30)
 * NOTE: ALWAYS resident
 * NOTE: Present on file or directory record.
 */

// Namespace for file name (1 byte)
typedef enum __attribute__((__packed__)) file_namespace {
    FILE_NAME_POSIX = 0x00, // Case sensitive and allows all unicode characters
                            // except null and '/'
    FILE_NAME_WIN32 = 0x01, // Case sensitive and allows all unicode characters
                            // except '\0', '"', '*', '/', ':', '<', '>', '?',
                            // '\', '|'. Names cannot end with '.' or space
    FILE_NAME_DOS =
        0x02, // Uppercase only and only allows 8-bit characters, except '"',
              // '*', '+', ',', '/', ':', ';', '<', '=', '>', '?', '\'
    FILE_NAME_WIN32_AND_DOS = 0x03, // Supports both WIN32 and DOS namespaces
} file_namespace_t;

/*
 * $FILE_NAME struct.
 * NOTE: Fields are only updated when file name is changed, except for
 *       parent_directory
 */
typedef struct __attribute__((__packed__)) mtf_file_name {
    uint64_t parent_directory;   // MTF entry for directory where this file is
                                 // referenced from
    time_values_t time;          // Common time values
    uint64_t      alloc_bytes;   // Bytes allocated on disk by the unnamed
                                 // $DATA. For a directory entry, this is 0
    uint64_t bytes;              // Actual size of the $DATA content,
                                 // regardless compressed or not. For a
                                 // directory entry, this is 0
    file_attr_t      file_flags; // File attributes
    uint32_t         reparse_value; // Present if no extended attributes
    uint8_t          name_len;      // Length of the file name
    file_namespace_t name_flags;    // How to interpret file name
    wchar_t          name[];        // UTF-16 file name
} mtf_file_name_t;
static_assert(sizeof(mtf_file_name_t) == 66);

template <typename T = mtf_std_info_t> class Content {};
} // namespace nf::NTFS
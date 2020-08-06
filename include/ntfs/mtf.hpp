// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * MTF related classes.
 */

#include <vector>

#include <ntfs/device.hpp>
#include <ntfs/mtf_attributes.hpp>

using namespace nf::NTFS;

/*
 * MTFAttribute class with helper methods
 */
class MTFAttribute {
  public:
    MTFAttribute(char *data, Device *dev);
    //~MTFAttribute();

    // Read n clusters from content. If end reached, stop
    char *read_content(uint32_t clusters = 1);

    /*
     * Getters and setters
     */

    // Type of attribute
    mtf_attr_kind_t type();

    // Resident or non resident
    bool resident();

    // Size in bytes of content
    uint32_t content_size();

    // Size in bytes of attribute
    uint32_t size();

    // Name of attribute
    std::string name();

  private:
    mtf_attr_header_t *_header;
    mtf_attr_kind_t    _type;

    Device * _dev;
    uint64_t _pos = 0;
};

/*
 * MTF entry class
 */
class MTFEntry {
  public:
    MTFEntry(char *data, Device *dev);
    //~MTFEntry();

    // Check if entry is a valid one
    static int check_header(MTFEntry entry);

    /*
     * Getters and setters
     */

    // Size of the attribute
    uint32_t size();

    // Unique ID of record
    uint64_t id();

  private:
    const char *  _data;
    mtf_header_t *_header;
    Device *      _dev;

    std::vector<MTFAttribute *> _attrs;
};
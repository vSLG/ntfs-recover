// SPDX-License-Identifier: LGPL-3.0-or-later

/*
 * implementation of device consistency checks.
 */

#include <log.hpp>
#include <ntfs/bootsector.hpp>
#include <ntfs/device.hpp>

using namespace nf::NTFS;

/*
 * Check if partition has valid NTFS signature.
 */
int Device::check_signature(nbs_t *nbs) {
    if (NTFSMagic != std::string((char *) nbs->oem)) {
        W << "This partition does not seem to be NTFS or it is corrupt.";
        return 1;
    }
    return 0;
}

/*
 * Check if reported bytes per sector is valid
 */
int Device::check_sector_size(nbs_t *nbs) {
    int warns = 0;

    if ((nbs->bpb.bps != 0) && ((nbs->bpb.bps & (nbs->bpb.bps - 1)) == 0))
        this->bpb->bps = nbs->bpb.bps;
    else {
        W << "Wrong sector size reported, guessing 512.";
        warns++;
    }

    if ((nbs->bpb.spc != 0) && (this->bpb->bps % nbs->bpb.spc == 0))
        this->bpb->spc = nbs->bpb.spc;
    else {
        W << "Wrong number of sectors per cluster reported, guessing 4.";
        warns++;
    }

    return warns;
}

/*
 * Check if reported partition size is beyound our seek capability.
 * This may sign NBS is corrupt or outdated even if FS sign is valid.
 */
int Device::check_partition_size(nbs_t *nbs) {
    size_t current = ftello64(this->_dev);
    fseeko64(this->_dev, 0, SEEK_END);
    size_t bytes_detected = ftello64(this->_dev) - 512l;
    fseeko64(this->_dev, current, SEEK_SET);

    size_t reported_size = this->bpb->bps * nbs->bpb.ts;

    if (reported_size == 0 || reported_size > bytes_detected) {
        W << "Partition size may be wrong.\n\tReported size: "
          << this->bpb->bps * nbs->bpb.ts
          << " bytes\n\tDetected size: " << bytes_detected << " bytes.";

        this->bpb->ts = bytes_detected / this->bpb->bps;
        return 1;
    }

    this->bpb->ts = nbs->bpb.ts;
    return 0;
}

/*
 * Check whether MTF cluster is within seek range and it is valid.
 */
int Device::check_mtf_cluster(nbs_t *nbs) {
    int      warns          = 0;
    uint64_t total_clusters = this->bpb->ts / this->bpb->spc;

    if (nbs->ntfs_header.mtf > total_clusters || nbs->ntfs_header.mtf == 0) {
        W << "Reported $MTF cluster is invalid or outside read "
             "bounds.\n\tReported $MTFmir cluster : "
          << nbs->ntfs_header.mtf2
          << "\n\tDetected clusters: " << total_clusters;
        warns++;
    } else
        this->ntfs_header->mtf = nbs->ntfs_header.mtf;

    if (nbs->ntfs_header.mtf2 > total_clusters || nbs->ntfs_header.mtf2 == 0) {
        W << "Reported $MTFmir cluster is invalid or outside read "
             "bounds.\n\tReported $MTFmir cluster : "
          << nbs->ntfs_header.mtf2
          << "\n\tDetected clusters: " << total_clusters;
        warns++;
    } else
        this->ntfs_header->mtf2 = nbs->ntfs_header.mtf2;

    return warns;
}
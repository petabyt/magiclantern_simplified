#ifndef __PLATFORM_MMU_PATCHES_H__
#define __PLATFORM_MMU_PATCHES_H__

#include "patch_mmu.h"

static const unsigned char earl_grey_str[] = "Earl Grey, hot";
static const unsigned char engage_str[] = "Engage!";

#if CONFIG_FW_VERSION == 101 // ensure our hard-coded patch addresses are not broken
                             // by a FW upgrade
struct region_patch mmu_patches[] =
{
    {
        // replace "Dust Delete Data" with "Earl Grey, hot",
        // as a low risk (non-code) test that MMU remapping works.
        .patch_addr = 0xf00d84e7,
        .orig_content = NULL,
        .patch_content = earl_grey_str,
        .size = sizeof(earl_grey_str),
        .description = "Tea"
    },
/*
    {
        // replace "High ISO speed NR" with "Engage!",
        // as a low risk (non-code) test that MMU remapping works.
        .patch_addr = 0xf0048842,
        .orig_content = NULL,
        .patch_content = engage_str,
        .size = sizeof(engage_str),
        .description = "GO!"
    }
*/
};

// a set of 64kB pages, for copying ROM contents into,
// so they can be modified.  These do not need to be
// contiguous.
struct mmu_ram_page mmu_ram_patch_pages[] =
{
    {0x43920000, 0},
    {0x43930000, 0},
    {0x43940000, 0},
};

#endif // 200D FW_VERSION 101

#endif // __PLATFORM_MMU_PATCHES_H__

#ifndef __PLATFORM_MMU_PATCHES_H__
#define __PLATFORM_MMU_PATCHES_H__

#include "patch_mmu.h"

static const unsigned char earl_grey_str[] = "Earl Grey, hot";
static const unsigned char engage_str[] = "Engage!";

#if CONFIG_FW_VERSION == 100 // ensure our hard-coded patch addresses are not broken
                             // by a FW upgrade
struct region_patch mmu_patches[] =
{
    {
        // replace "Lens aberration correction" with "Earl Grey, hot",
        // as a low risk (non-code) test that MMU remapping works.
        .patch_addr = 0xf05d79d1,
        .orig_content = NULL,
        .patch_content = earl_grey_str,
        .size = sizeof(earl_grey_str),
        .description = "Tea"
    },
    {
        // replace "Flash control" with "Engage!",
        // as a low risk (non-code) test that MMU remapping works.
        .patch_addr = 0xf0602a08,
        .orig_content = NULL,
        .patch_content = engage_str,
        .size = sizeof(engage_str),
        .description = "GO!"
    }
};

#endif // 850D FW_VERSION 100

#endif // __PLATFORM_MMU_PATCHES_H__

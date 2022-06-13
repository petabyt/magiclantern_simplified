#ifndef __PLATFORM_MMU_PATCHES_H__
#define __PLATFORM_MMU_PATCHES_H__

#include "patch_mmu.h"

static const unsigned char earl_grey_str[] = "Earl Grey, hot";
static const unsigned char engage_str[] = "Engage!";

#if CONFIG_FW_VERSION == 101 // ensure our hard-coded patch addresses are not broken
                             // by a FW upgrade
struct region_patch mmu_data_patches[] =
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
    {
        // replace "High ISO speed NR" with "Engage!",
        // as a low risk (non-code) test that MMU remapping works.
        .patch_addr = 0xf0048842,
        .orig_content = NULL,
        .patch_content = engage_str,
        .size = sizeof(engage_str),
        .description = "GO!"
    }
};

void __attribute__((noreturn,noinline,naked))hook_uart_printf(void)
{
    asm(
        // do overwritten instructions
        "push { r4-r6, lr }\n"
        "mov r5, r0\n"
        "add r2, sp, #0x14\n"
        "ldr r1, [sp, #0x10]\n"

        // jump back to uart_printf
        "ldr pc, =0xe04bdc69\n"
    );
}

struct function_hook_patch mmu_code_patches[] =
{
    {
        .patch_addr = 0xe04bdc60, // Slightly into uart_printf().
                                  // This avoids a pc relative instruction.
        .orig_content = {0x70, 0xb5, 0x05, 0x46, 0x05, 0xaa, 0x04, 0x99}, // used as a check before applying patch
        .target_function_addr = (uint32_t)hook_uart_printf,
        .description = "None"
    }
};

#endif // 200D FW_VERSION 101

#endif // __PLATFORM_MMU_PATCHES_H__

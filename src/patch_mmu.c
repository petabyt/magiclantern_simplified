// Memory patching, using MMU on supported cams.
// These cams can also do cache-hack patching,
// which is less complicated.

#include <dryos.h>
#include "patch_mmu.h"
#include <patch.h>
#include "mmu_utils.h"

#ifndef CONFIG_MMU
#error "Attempting to build patch_mmu.c but cam not listed as having an MMU - this is probably a mistake"
#endif

#if defined(CONFIG_EARLY_MMU_REMAP) && !defined(CONFIG_MMU_REMAP)
#error "You shouldn't have early MMU remap without MMU remap"
#endif

static uint32_t *tt_active = NULL; // Pointer to current MMU translation tables.
static uint32_t *tt_inactive = NULL; // Used when editing tables.
static uint32_t mmu_remap_initialised = 0;

extern void *memcpy_dryos(void *dst, void *src, uint32_t count);

#ifdef CONFIG_200D
// SJE FIXME quick hack put 200D test patches in here,
// these should live in per cam dir really.
static const unsigned char earl_grey_str[] = "Earl Grey, hot";
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
    }
};

#endif

int patch_region(struct region_patch *patch)
{
    // SJE FIXME currently this is hideous 200D hard-coded crap,
    // this is dev work as we make patching more generic

    if (mmu_remap_initialised == 0)
        return -1; // has init_remap_mmu() been called previously?

    uint32_t rom_base_addr = ROMBASEADDR & 0xff000000;
    // get original rom and ram memory flags
    uint32_t flags_rom = get_l2_largepage_flags_from_l1_section(rom_base_addr, CANON_ORIG_MMU_TABLE_ADDR);
    uint32_t flags_ram = get_l2_largepage_flags_from_l1_section(0x10000000, CANON_ORIG_MMU_TABLE_ADDR);
    // determine flags for our L2 page to give it RAM cache attributes
    uint32_t flags_new = flags_rom & ~L2_LARGEPAGE_MEMTYPE_MASK;
    flags_new |= (flags_ram & L2_LARGEPAGE_MEMTYPE_MASK);

    // Split 16MB Supersection containing target addr into 16 Sections, in our copied table.
    // We remap from start of rom for one section, e.g.:
    //      0xe000.0000 to 0xe010.0000 can be remapped.

    // SJE TODO - do all the mmu_utils.c calls work safely if we call them
    // twice, especially with addresses in the same section?
    uint32_t aligned_patch_addr = patch->patch_addr & 0xffff0000;
    split_l1_supersection(aligned_patch_addr, ML_MMU_TABLE_01_ADDR);

    // edit copy, pointing existing ROM code to our RAM versions
    replace_section_with_l2_tbl(aligned_patch_addr,
                                ML_MMU_TABLE_01_ADDR,
                                ML_MMU_L2_TABLE_01_ADDR,
                                flags_new);

    // SJE quick hack test, try and replace a string from asset rom
    // f00d84e7 "Dust Delete Data"
    replace_rom_page(aligned_patch_addr,
                     ML_MMU_64k_PAGE_01,
                     ML_MMU_L2_TABLE_01_ADDR,
                     flags_new);

    // Copy whole page ROM -> RAM
    memcpy_dryos((uint32_t *)ML_MMU_64k_PAGE_01,
                 (uint32_t *)aligned_patch_addr,
                 MMU_PAGE_SIZE);

    // Edit patch region in RAM copy
    memcpy_dryos((void *)(ML_MMU_64k_PAGE_01 + (patch->patch_addr & 0xffff)),
                 (void *)(patch->patch_content),
                 patch->size);

    // sync caches over edited table region
    dcache_clean(ML_MMU_64k_PAGE_01, MMU_PAGE_SIZE);
    dcache_clean(aligned_patch_addr, MMU_PAGE_SIZE);
    dcache_clean(ML_MMU_L2_TABLE_01_ADDR, 0x400);
    dcache_clean_multicore(ML_MMU_L2_TABLE_01_ADDR, 0x400);

    // flush icache
//    icache_invalidate(virt_addr, MMU_PAGE_SIZE);

    dcache_clean(ML_MMU_TABLE_01_ADDR, MMU_TABLE_SIZE);
    dcache_clean_multicore(ML_MMU_TABLE_01_ADDR, MMU_TABLE_SIZE);
    // 
    return 0;
}

int insert_hook_code_thumb_mmu(uintptr_t patch_addr, uintptr_t target_function, const char *description)
{
    // Patches Thumb code, to add a hook to a function inside ML code.
    //
    // That ML function may choose to execute the stored instructions
    // that we patched over, or may not.  That is to say: you can replace
    // the code, or augment it.  You are responsible for ensuring registers,
    // state etc make sense.
    //
    // The hook takes 8 bytes, and we don't handle any PC relative accesses,
    // so you must either fix that up yourself, or not patch over
    // instructions that use PC relative addressing.
    //
    // This is somewhat similar to patch.c patch_hook_function() but
    // with a clearer name and backed by MMU.

    // ensure page is remapped in the TT we will swap to
    if (remap_page(patch_addr, 8, tt_active))
        return -1; // SJE FIXME extend E_PATCH_* with MMU errors, see error_msg() in patch.c

    
    // schedule TTBR change(s)

    return 0;
}

#ifdef CONFIG_MMU_REMAP

extern uint32_t copy_mmu_tables(uint32_t dest_addr);
extern void change_mmu_tables(uint32_t ttbr0_address, uint32_t ttbr1_address, uint32_t cpu_id);
void init_remap_mmu(void)
{
    uint32_t cpu_id = get_cpu_id();
    uint32_t rom_base_addr = ROMBASEADDR & 0xff000000;

#if 0
    // Both CPUs want to use the updated MMU tables, but
    // only one wants to do the setup.
    if (cpu_id == 0)
    {
        if (mmu_remap_initialised == 0)
        {
            tt_active = (uint32_t *)ML_MMU_TABLE_01_ADDR;
            tt_inactive = (uint32_t *)ML_MMU_TABLE_02_ADDR;

            copy_mmu_tables_ex(ML_MMU_TABLE_01_ADDR,
                               rom_base_addr,
                               MMU_TABLE_SIZE);

//            apply_mmu_patches(ML_MMU_TABLE_01_ADDR);

            copy_mmu_tables_ex(ML_MMU_TABLE_01_ADDR,
                               ML_MMU_TABLE_02_ADDR,
                               MMU_TABLE_SIZE);

            // trigger cpu1 remap
        }
    }
    else
    {
        // wait until mmu_remap_initialised, update TTBRs
        while(mmu_remap_initialised == 0)
        {
            // Can we msleep() in CONFIG_EARLY_MMU_REMAP context?
            msleep(100);
        }
    }
#endif

#ifdef CONFIG_200D
    // copy original table to ram copy
    //
    // We can't use a simple copy, the table stores absolute addrs
    // related to where it is located.  There's a DryOS func that
    // does copy + address fixups
    int32_t align_fail = copy_mmu_tables_ex(ML_MMU_TABLE_01_ADDR,
                                            rom_base_addr,
                                            MMU_TABLE_SIZE);
    if (align_fail != 0)
        while(1); // maybe we can jump to Canon fw instead?

    // SJE FIXME hack, this block bodges enough init to test
    // patch_region()
    tt_inactive = (uint32_t *)ML_MMU_TABLE_01_ADDR;
    tt_active = tt_inactive;
    mmu_remap_initialised = 1;

    if (patch_region(&mmu_patches[0]) != 0)
        while(1);

    // update TTBRs (this DryOS function also triggers TLBIALL)
    uint32_t cpu_mmu_offset = MMU_TABLE_SIZE - 0x100 + cpu_id * 0x80;
    change_mmu_tables(ML_MMU_TABLE_01_ADDR + cpu_mmu_offset,
                      ML_MMU_TABLE_01_ADDR,
                      cpu_id);
#endif // CONFIG_200D
    return;
}
#endif // CONFIG_MMU_REMAP

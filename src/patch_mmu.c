// Memory patching, using MMU on supported cams.
// These cams can also do cache-hack patching,
// which is less complicated.
#ifdef CONFIG_MMU_REMAP

#include <dryos.h>
#include "patch_mmu.h"
#include <patch.h>
#include "mmu_utils.h"

#ifndef CONFIG_DIGIC_78
#error "So far, we've only seen MMU on Digic 7 and up.  This file makes that assumption re assembly, you'll need to fix something"
#endif

#ifndef CONFIG_MMU
#error "Attempting to build patch_mmu.c but cam not listed as having an MMU - this is probably a mistake"
#endif

#if defined(CONFIG_EARLY_MMU_REMAP) && !defined(CONFIG_MMU_REMAP)
#error "You shouldn't have early MMU remap without MMU remap"
#endif
#include "platform/mmu_patches.h"


extern void *memcpy_dryos(void *dst, void *src, uint32_t count);

int patch_region(struct region_patch *patch, uint32_t l1_table_addr, uint32_t l2_table_addr)
{
    // SJE FIXME currently this is incomplete;
    // we don't handle checking if a ROM page is already mapped
    // and always tries to use the *same* RAM page to back any remap.
    //
    // THIS WILL BREAK if you try to map more than one 0x10000 region.

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
    split_l1_supersection(aligned_patch_addr, l1_table_addr);

    // edit copy, pointing existing ROM code to our RAM versions
    replace_section_with_l2_tbl(aligned_patch_addr,
                                l1_table_addr,
                                l2_table_addr,
                                flags_new);

    // Copy whole page ROM -> RAM, and remap
    replace_rom_page(aligned_patch_addr,
                     mmu_ram_patch_pages[0].phys_addr,
                     l2_table_addr,
                     flags_new);

    // Edit patch region in RAM copy
    memcpy_dryos((void *)(mmu_ram_patch_pages[0].phys_addr + (patch->patch_addr & 0xffff)),
                 (void *)(patch->patch_content),
                 patch->size);

    // sync caches over edited table region
    dcache_clean(mmu_ram_patch_pages[0].phys_addr, MMU_PAGE_SIZE);
    dcache_clean(aligned_patch_addr, MMU_PAGE_SIZE);
    dcache_clean(l2_table_addr, 0x400);
    dcache_clean_multicore(l2_table_addr, 0x400);

    // flush icache
//    icache_invalidate(virt_addr, MMU_PAGE_SIZE);

    dcache_clean(l1_table_addr, MMU_TABLE_SIZE);
    dcache_clean_multicore(l1_table_addr, MMU_TABLE_SIZE);
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
    //if (remap_page(patch_addr, 8, tt_active))
    //    return -1; // SJE FIXME extend E_PATCH_* with MMU errors, see error_msg() in patch.c

    
    // schedule TTBR change(s)

    return 0;
}


extern uint32_t copy_mmu_tables(uint32_t dest_addr);
extern void change_mmu_tables(uint32_t ttbr0_address, uint32_t ttbr1_address, uint32_t cpu_id);
void init_remap_mmu(void)
{
    static uint32_t tt_active = ML_MMU_TABLE_01_ADDR; // Address of MMU translation tables that are in use.
    static uint32_t tt_l2_active = ML_MMU_L2_TABLE_01_ADDR;
    static uint32_t tt_inactive = ML_MMU_TABLE_02_ADDR; // Address of secondary tables, used when swapping.
    static uint32_t tt_l2_inactive = ML_MMU_L2_TABLE_02_ADDR;
    static uint32_t mmu_remap_cpu0_init = 0;
    static uint32_t mmu_remap_cpu1_init = 0;

    uint32_t cpu_id = get_cpu_id();
    uint32_t cpu_mmu_offset = MMU_TABLE_SIZE - 0x100 + cpu_id * 0x80;
    uint32_t rom_base_addr = ROMBASEADDR & 0xff000000;

    // Both CPUs want to use the updated MMU tables, but
    // only one wants to do the setup.
    if (cpu_id == 0)
    {
        if (mmu_remap_cpu0_init == 0)
        {
            // copy original table to ram copy
            //
            // We can't use a simple copy, the table stores absolute addrs
            // related to where it is located.  There's a DryOS func that
            // does copy + address fixups
            int32_t align_fail = copy_mmu_tables_ex(tt_active,
                                                    rom_base_addr,
                                                    MMU_TABLE_SIZE);
            if (align_fail != 0)
                while(1); // maybe we can jump to Canon fw instead?
            align_fail = copy_mmu_tables_ex(tt_inactive,
                                            rom_base_addr,
                                            MMU_TABLE_SIZE);
            if (align_fail != 0)
                while(1); // maybe we can jump to Canon fw instead?

            mmu_remap_cpu0_init = 1;

            for(uint32_t i = 0; i < COUNT(mmu_patches); i++)
            {
                if (patch_region(&mmu_patches[i], tt_active, tt_l2_active) != 0)
                    while(1);
                if (patch_region(&mmu_patches[i], tt_inactive, tt_l2_inactive) != 0)
                    while(1);
            }

            // update TTBRs (this DryOS function also triggers TLBIALL)
            change_mmu_tables(tt_active + cpu_mmu_offset,
                              tt_active,
                              cpu_id);
        }
    }
    else
    {
        if (mmu_remap_cpu1_init == 0)
        {
            while(mmu_remap_cpu0_init == 0)
            {
                // Can we msleep() in CONFIG_EARLY_MMU_REMAP context?
                msleep(100);
            }
            // update TTBRs
            // update TTBRs (this DryOS function also triggers TLBIALL)
            change_mmu_tables(tt_active + cpu_mmu_offset,
                              tt_active,
                              cpu_id);
            mmu_remap_cpu1_init = 1;
        }
    }

    return;
}
#endif // CONFIG_MMU_REMAP

// Memory patching, using MMU on supported cams.
// These cams can also do cache-hack patching,
// which is less complicated.

#include <dryos.h>
#include "patch_mmu.h"
#include <patch.h>
#include "mmu_utils.h"

#ifdef CONFIG_MMU_REMAP

#ifndef CONFIG_DIGIC_78
#error "So far, we've only seen MMU on Digic 7 and up.  This file makes that assumption re assembly, you'll need to fix something"
#endif

#ifndef CONFIG_MMU
#error "Attempting to build patch_mmu.c but cam not listed as having an MMU - this is probably a mistake"
#endif

#include "platform/mmu_patches.h"

extern void *memcpy_dryos(void *dst, const void *src, uint32_t count);
extern void early_printf(char *fmt, ...);

int assign_64k_to_L2_table(struct region_patch *patch,
                            struct mmu_L2_page_info *L2_page)
{
    // SJE TODO can / should we use a semaphore here?  I'm not sure we can
    // given the fairly early context we want to call this.

    // This index is how far into our set of available 64k RAM pages
    // we are.  When exhausted, we have no more RAM to back ROM edits.
    static uint32_t backing_page_index = 0;

    // An L2 section is split into 64kB "large pages", of which there can be 16
    // in use.  Determine which of these wants to be used.
    uint32_t i = patch->patch_addr & 0x000f0000;
    i >>= 16;
    qprintf("L2 backing page index: 0x%08x\n", i);

    // If possible, use that page, else, bail (we have run out of
    // available RAM to use for backing ROM edits).
    uint8_t **pages = L2_page->phys_mem;
    if (pages[i] == NULL)
    {
        if (backing_page_index >= MMU_MAX_64k_PAGES_REMAPPED)
            return -1;

        pages[i] = (uint8_t *)(MMU_64k_PAGES_START_ADDR + MMU_PAGE_SIZE * backing_page_index);
        qprintf("Rom->Ram copy: 0x%08x, 0x%08x\n", pages[i], patch->patch_addr & 0xffff0000);
        memcpy_dryos(pages[i],
                     (uint8_t *)(patch->patch_addr & 0xffff0000),
                     MMU_PAGE_SIZE);
        backing_page_index++;
    }
    else
    { // We hit a page already allocated by a previous patch
      // and can re-use it.
        return 0;
    }
    return 0;
}

// Given an array of possible pages, some of which may be in use,
// determine which page should be used for a given patch.
// 
// This is to make patches in the same 64kB region use the same
// page in ram.
//
// Returns info on the L2 table that should be use for the patch.
// Which ram page within that region can be determined by masking
// the patch address, an L2 table handles 1MB of ram and can have
// up to 16 associated 64k ram pages assigned (depending on
// MMU_MAX_64k_PAGES_REMAPPED).
//
// Returns NULL if no page can be found; this means there are too
// many patches for the pages, all are in use and the patch
// doesn't share an address.  Or, that a patch spans a 64kB
// boundary, this is not handled.
struct mmu_L2_page_info *find_L2_for_patch(struct region_patch *patch,
                                           struct mmu_L2_page_info *l2_pages,
                                           uint32_t num_pages)
{
    // L2 tables cover 1MB of remapped mem each

    if (num_pages == 0)
        return NULL;

    // check our patch doesn't span two 64kB pages, this is unhandled (currently)
    if (patch->patch_addr + patch->size > ((patch->patch_addr & 0xffff0000) + MMU_PAGE_SIZE))
        return NULL;

    // loop backwards so we assign unused pages forwards,
    // meaning if called on a sorted set of pages they're
    // assigned sorted by address.
    struct mmu_L2_page_info *unused_page = NULL;
    struct mmu_L2_page_info *l2_page = NULL;
    for(uint32_t i = num_pages; i != 0; i--)
    {
        l2_page = (l2_pages + i - 1);
        if (l2_page->virt_page_mapped == (patch->patch_addr & 0xfff00000) &&
            l2_page->in_use)
        {
            if (assign_64k_to_L2_table(patch, l2_page) == 0)
                return l2_page;
            else
                return NULL;
        }
        else if (l2_page->in_use == 0)
        {
            unused_page = l2_page;
        }
    }
    if(unused_page == NULL) // no matching pages and no free pages; too many patches
    {
        //early_printf("Too many distinct patch addresses for available patch pages\n");
        return NULL;
    }

    // page was free, no matches found, use this page
    if (assign_64k_to_L2_table(patch, unused_page) == 0)
    {
        unused_page->virt_page_mapped = patch->patch_addr & 0xfff00000;
        return unused_page;
    }
    return NULL; // could not assign 64k page, probably exhausted pool of pages
}

// Given info on a set of translation tables, and info about a patch,
// update the tables to redirect accesses to some address range to
// a copy held in ram.  This copy will be edited as described by the patch.
// Note this does not instruct the CPU to use the new tables.
//
// The patch struct describes what address should be patched,
// individual patches can be large (up to 64kB).
//
// NB this function doesn't update TTBR registers.
// It does invalidate relevant cache entries.
// It *doesn't* do this in a technically safe way;
// We don't disable interrupts etc, so cache info could go
// stale before we update TTBRs.
// FIXME - probably with locks, cli/sei wrapping,
// including getting cpu0 to sleep cpu1 before, wake after.
// Note even then that's not fully safe; a task may be scheduled
// that is paused, but executing code that is about to be patched.
// It would wake up to find the instructions had altered and state
// was inconsistent.  This is hard to handle reliably.
int apply_data_patch(struct mmu_config *mmu_conf,
                     struct region_patch *patch)
{
    uint32_t rom_base_addr = ROMBASEADDR & 0xff000000;
    // get original rom and ram memory flags
    uint32_t flags_rom = get_l2_largepage_flags_from_l1_section(rom_base_addr, CANON_ORIG_MMU_TABLE_ADDR);
    uint32_t flags_ram = get_l2_largepage_flags_from_l1_section(0x10000000, CANON_ORIG_MMU_TABLE_ADDR);
    // determine flags for our L2 page to give it RAM cache attributes
    uint32_t flags_new = flags_rom & ~L2_LARGEPAGE_MEMTYPE_MASK;
    flags_new |= (flags_ram & L2_LARGEPAGE_MEMTYPE_MASK);

    uint32_t aligned_patch_addr = patch->patch_addr & 0xffff0000;

    struct mmu_L2_page_info *target_page = find_L2_for_patch(patch,
                                                             mmu_conf->L2_tables,
                                                             MMU_MAX_L2_TABLES);

    if (target_page == NULL)
    {
        qprintf("Target page NULL: 0x%08x\n", patch);
        return -1;
    }

    // add page to tables
    qprintf("Doing TT edit: 0x%08x\n", aligned_patch_addr);
    qprintf("Target L2: 0x%08x\n", target_page->l2_mem);

    qprintf("Splitting L1 for: 0x%08x\n", patch->patch_addr);
    // point containing L1 table entry to our L2
    split_l1_supersection(patch->patch_addr, (uint32_t)mmu_conf->L1_table);
    if (target_page->in_use == 0)
    { // this wipes the L2 table so we must only do it the first time
      // we map a page in this section
        replace_section_with_l2_tbl(patch->patch_addr,
                                    (uint32_t)mmu_conf->L1_table,
                                    (uint32_t)target_page->l2_mem,
                                    flags_new);
        target_page->in_use = 1;
    }

    // Remap ROM page in RAM
    uint32_t i = patch->patch_addr & 0x000f0000;
    i >>= 16;
    qprintf("Phys mem: 0x%08x\n", target_page->phys_mem[i]);
    replace_rom_page(aligned_patch_addr,
                     (uint32_t)target_page->phys_mem[i],
                     (uint32_t)target_page->l2_mem,
                     flags_new);

    // Edit patch region in RAM copy
    memcpy_dryos(target_page->phys_mem[i] + (patch->patch_addr & 0xffff),
                 patch->patch_content,
                 patch->size);

    // sync caches over edited table region
    dcache_clean((uint32_t)target_page->l2_mem, MMU_L2_TABLE_SIZE);
    dcache_clean_multicore((uint32_t)target_page->l2_mem, MMU_L2_TABLE_SIZE);

    // ensure icache takes new code if relevant
    icache_invalidate(patch->patch_addr, MMU_PAGE_SIZE);

    dcache_clean((uint32_t)mmu_conf->L1_table, MMU_TABLE_SIZE);
    dcache_clean_multicore((uint32_t)mmu_conf->L1_table, MMU_TABLE_SIZE);

    dcache_clean((uint32_t)target_page->phys_mem[i], MMU_PAGE_SIZE);
    dcache_clean(aligned_patch_addr, MMU_PAGE_SIZE);

    return 0;
}

int apply_code_patch(struct mmu_config *mmu_conf,
                     struct function_hook_patch *patch)
{
    // confirm orig_content matches
    for (uint32_t i = 0; i < 8; i++)
    {
        if (*(uint8_t *)(patch->patch_addr + i) != *(patch->orig_content + i))
            return -1;
    }

    // our hook is 4 bytes for mov pc, [pc + 4]: 0xf000f8df
    // followed by 4 bytes of address for the target
    const uint8_t hook[8] = {0xdf, 0xf8, 0x00, 0xf0,
                             (uint8_t)(patch->target_function_addr & 0xff),
                             (uint8_t)((patch->target_function_addr >> 8) & 0xff),
                             (uint8_t)((patch->target_function_addr >> 16) & 0xff),
                             (uint8_t)((patch->target_function_addr >> 24) & 0xff)};

    // create data patch to apply our hook
    struct region_patch hook_patch = {
        .patch_addr = patch->patch_addr,
        .orig_content = NULL,
        .patch_content = hook,
        .size = 8,
        .description = NULL
    };

    return apply_data_patch(mmu_conf, &hook_patch);
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
extern void change_mmu_tables(uint8_t *ttbr0, uint8_t *ttbr1, uint32_t cpu_id);
// Sets up structures required for remapping via MMU,
// and applies compile-time specified patches from platform/XXD/include/platform/mmu_patches.h
void init_remap_mmu(void)
{
    // SJE FIXME I don't like all the while(1) error "handling".
    // I did this partly because we can't signal well here, partly because
    // this code used to be in bootloader context where we can't signal well at all.
    // Could change this to return int and handle errors at a higher level.
    // Could use early_printf().
    // Could fix the old code for giving a GUI error for early problems.
    static uint32_t mmu_remap_cpu0_init = 0;
    static uint32_t mmu_remap_cpu1_init = 0;

    static struct mmu_config mmu_config_active = {NULL, NULL};
//    static struct mmu_config mmu_config_inactive = {NULL, NULL};

    uint32_t cpu_id = get_cpu_id();
    uint32_t cpu_mmu_offset = MMU_TABLE_SIZE - 0x100 + cpu_id * 0x80;
    uint32_t rom_base_addr = ROMBASEADDR & 0xff000000;

    // Both CPUs want to use the updated MMU tables, but
    // only one wants to do the setup.
    if (cpu_id == 0)
    {
        // Technically, the following check is not race safe.
        // Don't call it twice from the same CPU, in simultaneously scheduled tasks.
        if (mmu_remap_cpu0_init == 0)
        {
            mmu_remap_cpu0_init = 1;
            mmu_config_active.L1_table = (uint8_t *)MMU_L1_TABLE_01_ADDR;
//            mmu_config_inactive.L1_table = (uint8_t *)MMU_L1_TABLE_02_ADDR;
            mmu_config_active.L2_tables = (struct mmu_L2_page_info *)MMU_L2_PAGES_INFO_START_ADDR;
//            mmu_config_inactive.L2_tables = (struct mmu_L2_page_info *)(MMU_L2_PAGES_INFO_START_ADDR
//                                            + sizeof(struct mmu_L2_page_info) * MMU_MAX_L2_TABLES);
            // Copy original table to ram copies.
            //
            // We can't use a simple copy, the table stores absolute addrs
            // related to where it is located.  There's a DryOS func that
            // does copy + address fixups, but that hardcodes 0xe000.0000 as the src.
            int32_t align_fail = copy_mmu_tables_ex((uint32_t)mmu_config_active.L1_table,
                                                    rom_base_addr,
                                                    MMU_TABLE_SIZE);
            if (align_fail != 0)
                while(1); // maybe we can jump to Canon fw instead?
/*
            align_fail = copy_mmu_tables_ex((uint32_t)mmu_config_inactive.L1_table,
                                            rom_base_addr,
                                            MMU_TABLE_SIZE);
            if (align_fail != 0)
                while(1); // maybe we can jump to Canon fw instead?
*/

            // memset and calloc are not available this early, init our L2 page info manually
            uint8_t *mmu_L2_tables = (uint8_t *)MMU_L2_TABLES_START_ADDR;

            uint32_t i = 0;
            for (i = 0; i < MMU_MAX_L2_TABLES; i++)
            {
                for (uint8_t j = 0; j < 16; j++)
                {
                    mmu_config_active.L2_tables[i].phys_mem[j] = NULL;
//                        mmu_config_inactive.L2_tables[i].phys_mem[j] = NULL;
                }
                mmu_config_active.L2_tables[i].l2_mem = (mmu_L2_tables + (i * MMU_L2_TABLE_SIZE));
                mmu_config_active.L2_tables[i].virt_page_mapped = 0x0;
                mmu_config_active.L2_tables[i].in_use = 0x0;

//                    mmu_config_inactive.L2_tables[i].l2_mem = (mmu_L2_tables + ((i + MMU_MAX_L2_TABLES) * MMU_L2_TABLE_SIZE));
//                    mmu_config_inactive.L2_tables[i].virt_page_mapped = 0x0;
//                    mmu_config_inactive.L2_tables[i].in_use = 0x0;
            }

            for (i = 0; i != COUNT(mmu_data_patches); i++)
            {
                if (apply_data_patch(&mmu_config_active, &mmu_data_patches[i]) < 0)
                    while(1);
/*
                if (apply_data_patch(&mmu_config_inactive, &mmu_data_patches[i]) < 0)
                    while(1);
*/
            }

            for (i = 0; i != COUNT(mmu_code_patches); i++)
            {
                if (apply_code_patch(&mmu_config_active, &mmu_code_patches[i]) < 0)
                    while(1);
            }
            // SJE FIXME do inactive equiv for code patches here,
            // if we're doing that at all

            #ifdef CONFIG_QEMU
            // qprintf the results for debugging
            #endif

            // update TTBRs (this DryOS function also triggers TLBIALL)
            change_mmu_tables(mmu_config_active.L1_table + cpu_mmu_offset,
                              mmu_config_active.L1_table,
                              cpu_id);
        }
    }
    else
    {
        if (mmu_remap_cpu1_init == 0)
        {
            mmu_remap_cpu1_init = 1;
            while(mmu_remap_cpu0_init == 0)
            {
                msleep(100);
            }
            // update TTBRs
            // update TTBRs (this DryOS function also triggers TLBIALL)
            change_mmu_tables(mmu_config_active.L1_table + cpu_mmu_offset,
                              mmu_config_active.L1_table,
                              cpu_id);
        }
    }

    return;
}
#endif // CONFIG_MMU_REMAP

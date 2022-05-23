#ifndef _patch_mmu_h_
#define _patch_mmu_h_
// Memory patching, using MMU on supported cams.

#ifndef CONFIG_MMU
#error "Attempting to build patch_mmu.c but cam not listed as having an MMU - this is probably a mistake"
#endif

#ifndef CONFIG_MMU_REMAP
#error "Attempting to build patch_mmu.c but cam not listed as having support for MMU remapping - this is probably a mistake"
#endif

struct region_patch
{
    uint32_t patch_addr; // Address of start of edited content; the VA to patch.
                         // memcpy((void *)patch_addr, orig_content, size) would undo the patch,
                         // but unpatch_region() should be used, not memcpy directly, so that
                         // book-keeping of patches is handled correctly (and consistently
                         // with existing patch.c functions).
    uint8_t *orig_content; // Copy of original content, before patching.
    const uint8_t *patch_content; // Patch data that will overwrite orig data.
    uint32_t size; // Length of patched region in bytes.
    const char *description; // What is the patch for?  Shows in ML menus.
};

// Applies a patch to a region: but does not update TTBRs, so the patch is inactive.
// The reasoning for this is that frequently you want to apply multiple patches,
// then commit all at once with TTBR update.
//
// FIXME: this should probably do book-keeping stuff, see patch.c,
// stuff around patch/unpatch function pairs
int patch_region(struct region_patch *patch, uint32_t l1_table_addr, uint32_t l2_table_addr);

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
int insert_hook_code_thumb_mmu(uintptr_t patch_addr, uintptr_t target_function, const char *description);

void init_remap_mmu(void);

#endif // _patch_mmu_h_

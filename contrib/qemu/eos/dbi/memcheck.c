#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "cpu.h"

#include "hw/boards.h"
#include "../eos.h"
#include "../model_list.h"
#include "memcheck.h"

/* hardcoded; find it with:
 * readelf magiclantern -a | grep " memcpy$" */
static const int ml_memcpy = 0x0974c0;
static const int ml_memcpy_size = 0x96B50 - 0x969EC;

/* our own read-after-free, in __mem_free */
/* if wrong: lots of warning messages containing f12eeeed during ML module loading */
/* not needed if MEMCHECK_CHECK is undefined */
static const int ml_double_free_check = 0x500b0;

struct memcheck_stubs
{
    /* any malloc/free-like routines */
    uint32_t malloc[4];
    uint32_t free[4];

    /* any memcpy-like routines */
    uint32_t memcpy[4];
    uint32_t memcpy_end[4];

    /* routines for initializing a memory heap */
    uint32_t init_heap;
    uint32_t checked_heaps[4];

    /* routine for allocating from a heap */
    uint32_t heap_alloc;
    uint32_t heap_free;

    /* allow these routines to use-after-free */
    uint32_t heap_routines[8][2];
};

static struct memcheck_stubs stubs;

/* 2 bits for each address */
/* ull because it's shifted and used against a 64-bit value */
#define MS_NOINIT 1ull     /* not initialized */
#define MS_FREED  2ull     /* freed */
#define MS_MASK   3ull     /* mask for all of the above */
static uint64_t mem_status[1024*1024*1024/64];

static inline bool is_uninitialized(uint32_t addr)
{
    addr &= ~0x40000000;
    assert(addr < 0x20000000);
    uint32_t i = 2 * addr;
    return mem_status[i/64] & (MS_NOINIT << (i & 63));
}

static inline bool is_freed(uint32_t addr)
{
    addr &= ~0x40000000;
    assert(addr < 0x20000000);
    uint32_t i = 2 * addr;
    return mem_status[i/64] & (MS_FREED << (i & 63));
}

static inline void set_initialized(uint32_t addr)
{
    addr &= ~0x40000000;
    assert(addr < 0x20000000);
    uint32_t i = 2 * addr;
    mem_status[i/64] &= ~(MS_NOINIT << (i & 63));
}

static inline void set_uninitialized(uint32_t addr)
{
    addr &= ~0x40000000;
    assert(addr < 0x20000000);
    uint32_t i = 2 * addr;
    mem_status[i/64] |= (MS_NOINIT << (i & 63));
}

static inline void set_freed(uint32_t addr)
{
    addr &= ~0x40000000;
    assert(addr < 0x20000000);
    uint32_t i = 2 * addr;
    mem_status[i/64] |= (MS_FREED << (i & 63));
}

static inline void clr_freed(uint32_t addr)
{
    addr &= ~0x40000000;
    assert(addr < 0x20000000);
    uint32_t i = 2 * addr;
    mem_status[i/64] &= ~(MS_FREED << (i & 63));
}

static void mem_set_status(uint32_t start, uint32_t end, uint64_t status)
{
    start &= ~0x40000000;
    end   &= ~0x40000000;

    /* can be optimized; keep it simple for now */
    for (uint32_t addr = start; addr < end; addr++)
    {
        uint32_t i = 2 * addr;
        mem_status[i/64] &= ~(MS_MASK << (i & 63));
        mem_status[i/64] |= (status << (i & 63));
    }
}

static void copy_mem_status(uint32_t src, uint32_t dst, uint32_t size)
{
    src &= ~0x40000000;
    dst &= ~0x40000000;

    /* can be optimized; keep it simple for now */
    for (uint32_t i = 0; i < size; i++)
    {
        uint32_t si = 2 * (src + size);
        uint32_t di = 2 * (dst + size);
        uint64_t sm = (3 << (si & 63));
        uint64_t dm = (3 << (di & 63));
        mem_status[di/64] &= ~dm;
        mem_status[di/64] |= (mem_status[di/64] & sm);
    }
}


static int is_heap_routine(EOSState *s, uint32_t pc)
{
    /* malloc/free are allowed to use after free */

    for (int i = 0; i < COUNT(stubs.heap_routines); i++)
    {
        assert(stubs.heap_routines[i][0] <= stubs.heap_routines[i][1]);

        if (pc >= stubs.heap_routines[i][0] &&
            pc <= stubs.heap_routines[i][1])
        {
            return 1;
        }

    }

    if (pc == ml_double_free_check)
    {
        return 1;
    }

    return 0;
}

static int is_memcpy(EOSState *s, uint32_t pc)
{
    if ((pc >= ml_memcpy && pc <= ml_memcpy + ml_memcpy_size) ||
        (pc >= stubs.memcpy[0] && pc <= stubs.memcpy_end[0])  ||
        (pc >= stubs.memcpy[1] && pc <= stubs.memcpy_end[1])  ||
        (pc >= stubs.memcpy[2] && pc <= stubs.memcpy_end[2])  ||
        (pc >= stubs.memcpy[3] && pc <= stubs.memcpy_end[3]))
    {
        return 1;
    }

    return 0;
}

void eos_memcheck_log_mem(EOSState *s, hwaddr addr, uint64_t value, uint32_t size, int flags)
{
    int is_write = flags & 1;
    int no_check = flags & NOCHK_LOG;
    int pc = CURRENT_CPU->env.regs[15];
    int lr = CURRENT_CPU->env.regs[14];

    /* fixme: only first byte is checked */
    if (!no_check &&
        is_freed(addr) &&                   /* check for use after free */
        (is_write || !is_memcpy(s, pc)) &&  /* don't check memcpy for reads */
        !is_heap_routine(s, pc))            /* don't check malloc/free routines */
    {
        printf(KLRED"[%s:%x:%x] %x %s after free (%x)" KRESET "\n",
            eos_get_current_task_name(s), pc, lr,
            (int)addr, is_write ? "written" : "read", (int)value
        );
    }

    if (0 && addr < 0x1000 && pc > 0x1000)
    {
        printf(KLRED"[%s:%x:%x] %x %s TCM (%x)"KRESET"\n",
            eos_get_current_task_name(s), pc, lr,
            (int)addr, is_write ? "written to" : "read from", (int)value
        );
    }

    if (!is_write)
    {
        /* fixme: only first byte is checked */
        /* fixme: why do we have to exclude heap routines here? */
        if (!no_check &&
            is_uninitialized(addr) &&
            !is_freed(addr) &&
            !is_memcpy(s, pc) &&
            !is_heap_routine(s, pc))
        {
            printf(KLRED"[%s:%x:%x] %x uninitialized (%x)"KRESET"\n",
                eos_get_current_task_name(s), pc, lr,
                (int)addr, (int)value
            );
        }
    }
    else /* write */
    {
        /* set each byte as initialized; don't change freed status */
        switch (size)
        {
            case 4:
                set_initialized(addr+3);
                set_initialized(addr+2);
            case 2:
                set_initialized(addr+1);
            case 1:
                set_initialized(addr);
                break;
            default:
                assert(0);
        }
    }
}

/* execution hook */

static int malloc_lr[4] = {0};
static int malloc_size[4] = {0};

static int malloc_list_p[16384] = {0};
static int malloc_list_s[16384] = {0};
static int malloc_idx = 0;

/* fixme: many addresses hardcoded to 500D */
void eos_memcheck_log_exec(EOSState *s, uint32_t pc)
{
    /* for some reason, this may called multiple times on the same PC */
    static uint32_t prev_pc = 0xFFFFFFFF;
    if (prev_pc == pc) return;
    prev_pc = pc;

    /* our uninitialized stubs are 0 - don't log this address */
    if (!pc) return;

    /* allow a few multi-tasked calls */
    int id = (malloc_lr[0] == 0) ? 0 :
             (malloc_lr[1] == 0) ? 1 :
             (malloc_lr[2] == 0) ? 2 :
             (malloc_lr[3] == 0) ? 3 :
                                  -1 ;
    assert(id >= 0);

    if (pc == stubs.malloc[0] || pc == stubs.malloc[1] || pc == stubs.malloc[2] || pc == stubs.malloc[3])
    {
        assert(malloc_lr[id] == 0);
        malloc_lr[id] = CURRENT_CPU->env.regs[14];
        malloc_size[id] = CURRENT_CPU->env.regs[0];
        qemu_log_mask(EOS_LOG_VERBOSE, "[%x] malloc(%x) lr=%x\n", id, malloc_size[id], malloc_lr[id]);
    }

    for (id = 0; id < COUNT(malloc_lr); id++)
    {
        if (pc == malloc_lr[id])
        {
            malloc_lr[id] = 0;
            int malloc_ptr = CURRENT_CPU->env.regs[0];
            qemu_log_mask(EOS_LOG_VERBOSE, "[%x] malloc => %x\n", id, malloc_ptr);
            mem_set_status(malloc_ptr, malloc_ptr + malloc_size[id], MS_NOINIT);
            assert(is_uninitialized(malloc_ptr));

            while (malloc_list_p[malloc_idx])
            {
                malloc_idx++;

                if (malloc_idx == COUNT(malloc_list_p))
                {
                    malloc_idx = 0;
                }
            }
            malloc_list_p[malloc_idx] = malloc_ptr;
            malloc_list_s[malloc_idx] = malloc_size[id];
        }
    }

    if (pc == stubs.free[0] || pc == stubs.free[1] || pc == stubs.free[2] || pc == stubs.free[3])
    {
        int free_ptr = CURRENT_CPU->env.regs[0];
        qemu_log_mask(EOS_LOG_VERBOSE, "free %x ", free_ptr);
        for (int i = 0; i < COUNT(malloc_list_p); i++)
        {
            /* fixme: going backwards from malloc_idx may be faster on average */
            if (malloc_list_p[i] == free_ptr)
            {
                int size = malloc_list_s[i];
                qemu_log_mask(EOS_LOG_VERBOSE, "size %x\n", size);
                mem_set_status(free_ptr, free_ptr + size, MS_FREED | MS_NOINIT);
                assert(is_freed(free_ptr));
                malloc_list_p[i] = 0;
                malloc_list_s[i] = 0;
            }
        }
    }

    if (pc == stubs.memcpy[0] || pc == stubs.memcpy[1] || pc == stubs.memcpy[2] || pc == stubs.memcpy[3])
    {
        int dst = CURRENT_CPU->env.regs[0];
        int src = CURRENT_CPU->env.regs[1];
        int num = CURRENT_CPU->env.regs[2];
        int lr  = CURRENT_CPU->env.regs[14];
        qemu_log_mask(EOS_LOG_VERBOSE,
            "[%s:%x] memcpy %x %x %x\n",
            eos_get_current_task_name(s), lr,
            dst, src, num
        );
        copy_mem_status(src, dst, num);
    }

    if (pc == stubs.init_heap)   /* init_heap */
    {
        int start = CURRENT_CPU->env.regs[0];
        int size  = CURRENT_CPU->env.regs[1];
        fprintf(stderr, "init_heap %x %x\n", start, start+size);

        if (start == stubs.checked_heaps[0] ||
            start == stubs.checked_heaps[1] ||
            start == stubs.checked_heaps[2] ||
            start == stubs.checked_heaps[3])
        {
            fprintf(stderr, "Checking this heap.\n");
            mem_set_status(start, start+size, MS_FREED | MS_NOINIT);
        }
    }
}

void eos_memcheck_init(EOSState *s)
{
    printf("Marking all memory as uninitialized...\n");
    mem_set_status(0, s->model->ram_size, MS_NOINIT);
    /* fixme: also check both TCMs */

    /* todo: identify stubs in a way that does not require hardcoding? */
    if (strcmp(s->model->name, "500D") == 0)
    {
        stubs = (struct memcheck_stubs) {
            .malloc         = { 0xFF018F70, 0xFF06ACB4 },
            .free           = { 0xFF019044, 0xFF06B044 },
            .memcpy         = { 0xFF3EBBC4 },
            .memcpy_end     = { 0xFF3EBC2C },
            .init_heap      =   0xFF06A4CC,
            .checked_heaps  = { 0x300000 },

            .heap_routines  = {
                                { 0xff06a4f4, 0xFF06B5CC }, /* AllocateMemory */
                                { 0xFF018F70, 0xFF019110 }, /* malloc */
                                { 0xff06c1c0, 0xFF06C1EC }, /* PackMem? */
                                { 0xff06c15c, 0xFF06C188 }, /* PackMem get size? */
                                { 0xFF06CC4C, 0xFF06CCA8 }, /* GetNextMemoryChunk?! why? */
                                { 0xff06beec, 0xFF06BF98 }, /* DeleteMemorySuite?! why? */
                              }, 
        };
    }
    else
    {
        fprintf(stderr, "FIXME: no memcheck stubs for %s.\n", s->model->name);
    }
}

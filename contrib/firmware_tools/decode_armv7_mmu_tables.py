#!/usr/bin/env python3

# Based on a1ex's v7mmap.py: https://a1ex.magiclantern.fm/bleeding-edge/R5/v7mmap.py
# Also see srsa's post about MMU: https://www.magiclantern.fm/forum/index.php?topic=25399.0
#
# Fundamentally based upon "ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition",
# I believe this is the relevant pseudo-code:
# https://developer.arm.com/documentation/ddi0406/b/System-Level-Architecture/Virtual-Memory-System-Architecture--VMSA-/Pseudocode-details-of-VMSA-memory-system-operations/Translation-table-walk

import os
import sys
import argparse
from struct import unpack


def main():
    args = parse_args()

    ROM = open(args.rom, "rb").read()

    # What is TTBR?  This means Translation Table Base Register.
    # There are three,  TTBR0, TTBR1 and TTBCR.
    # TTBCR controls whether TTBR0 or TTBR1 is used to find the tables.
    #
    # The virtual address space can be split into two regions,
    # with TTBR0 defining the mappings for the lower addresses,
    # TTBR1 the upper.
    #
    # TTBR0 is saved/restored on context switches, TTBR1 is not.
    #
    # Canon sometimes sets these different per core.
    # On Digic 7, 8 and X dual-cores, we see Canon map a page of early
    # mem so each core sees VA 0x1000:0x2000 mapped to different
    # physical mem.  This is used for per core data.
    #
    # This also means VA 0 is unmapped and will fault on access,
    # which catches null pointer derefs.
    #
    # https://developer.arm.com/documentation/ddi0406/b/System-Level-Architecture/Virtual-Memory-System-Architecture--VMSA-/Translation-tables?lang=en

    # While some cams do set different values here, it doesn't seem
    # to make a difference when parsing.  I don't understand this yet,
    # I assume some alignment makes the low bits irrelevant.
    #
    # This holds cpu0 [ttbr0, ttbr1], cpu1 [ttbr0, ttbr1]
    cam_ttbrs = {"R5":  ([0xe0004800, 0xe0000000],
                         [0xe0004880, 0xe0000000]),
                 "M50": ([0xe0004800, 0xe0000080],
                         [0xe0004880, 0xe0000080])}

    for cpu_id in [0, 1]:
        print("CPU%d" % cpu_id)

        last_offset = None
        vstart = 0
        pstart = 0
        size = 0
        last_ap = None
        last_xn = None
        last_texcb = None

        # address, phys_addr, page_size, ap, texcb, xn
        for a, p, s, ap, texcb, xn in walk_ttbrs(ROM,
                                                 cam_ttbrs["R5"][cpu_id],
                                                 verbose=args.verbose):
            offset = p - a
            s = 1024
            if offset == last_offset and ap == last_ap and xn == last_xn and texcb == last_texcb and a == vstart + size:
                size += s
            else:
                if last_offset is not None:
                    print("%08X-%08X -> %08X-%08X (%+5X) %-4s %s %s" %
                                (vstart, vstart+size-1, pstart, pstart+size-1,
                                last_offset, decode_texcb(last_texcb), decode_ap(last_ap),
                                decode_xn(last_xn)))
                vstart, pstart, size = a, p, s
                last_offset = offset
                last_ap = ap
                last_xn = xn
                last_texcb = texcb

        print("%08X-%08X -> %08X-%08X (%+5X) %-4s %s %s" % 
                    (vstart, vstart+size-1, pstart, pstart+size-1,
                    last_offset, decode_texcb(last_texcb), decode_ap(last_ap),
                    decode_xn(last_xn)))
        print("")


def parse_args():
    description = """
    For cams that have MMU tables in rom (all dual core D78X?),
    dump the VA -> PA mappings
    """

    parser = argparse.ArgumentParser(description=description)

    parser.add_argument("rom",
                        help="path to ROM to attempt MMU table parsing")
    parser.add_argument("-v", "--verbose",
                        default=False,
                        action="store_true")

    args = parser.parse_args()
    args.rom = os.path.realpath(args.rom)
    if not os.path.isfile(args.rom):
        print("ROM didn't exist: '%s'" % args.rom)
        sys.exit(-1)

    return args


def getLongLE(d, address):
   return unpack('<L',d[address:address+4])[0]


def getByte(d, address):
   return ord(d[address])


def getString(d, address):
    return d[address : d.index('\0', address)]


def extract32(value, start, length):
    assert(start >= 0 and length > 0 and length <= 32 - start);
    return (value >> start) & (0xFFFFFFFF >> (32 - length));


def walk_ttbrs(ROM, ttbrs, verbose=False):
    rombase = 0xE0000000
    for i, ttbr in enumerate(ttbrs):
        print("TTBR%d: %08X" % (i, ttbr))
        print("===============")
        if not ttbr:
            continue
        for address in range(0, 1<<32, 1024):
            tbl = 1 if address >> (32-7) else 0
            if tbl != i:
                continue

            base_mask = 0xffffc000 if i == 1 else 0xffffff80
            entry_addr = (ttbr & base_mask) | ((address >> 18) & 0x3ffc)
            #~ print hex(ttbr), hex(ttbr & base_mask), hex((address >> 18) & 0x3ffc), hex(entry_addr)
            desc = getLongLE(ROM, entry_addr - rombase)
            type = desc & 3
            if verbose:
                print(("%08X [%08X]: %08X %s" % (address, entry_addr, desc, bin(type|4)[3:])), end=' ')

            #~ print hex(address), hex(entry_addr), hex(desc), type
            if type == 1:
                domain = (desc >> 5) & 0x0f
                sbz = desc & 0x1E
                assert sbz == 0
                table_addr = (desc & 0xfffffc00)
                l2_entry = table_addr | ((address >> 10) & 0x3fc);
                desc = getLongLE(ROM, l2_entry - rombase)
                if verbose:
                    print("L2 table=%08X domain=%X entry=%X" % (table_addr, domain, l2_entry), end=' ')

                ap = ((desc >> 4) & 3) | ((desc >> 7) & 4);
                typ = (desc & 3)
                if typ == 0:
                    if verbose:
                        print(" -> fault")
                    continue
                elif typ == 1:
                    page_addr = (desc & 0xffff0000)
                    page_size = 0x10000;
                    phys_addr = page_addr | (address & 0xffff);
                    acc = desc & 0xffff
                    xn = desc & (1 << 15);
                    texcb = ((desc >> 2) & 3) | (desc >> 10) & 0x1C
                    if verbose:
                        print(" -> 64K page at %X -> %X" % (page_addr, phys_addr))
                elif typ == 2 or typ == 3:
                    page_addr = (desc & 0xfffff000)
                    page_size = 0x1000;
                    phys_addr = page_addr | (address & 0xfff);
                    acc = desc & 0xfff
                    xn = desc & 1;
                    texcb = ((desc >> 2) & 3) | (desc >> 4) & 0x1C
                    if verbose:
                        print(" -> 4K page at %X -> %X" % (page_addr, phys_addr))
                ap = ((desc >> 4) & 3) | ((desc >> 7) & 4);
            elif type == 2:
                if desc & (1 << 18):
                    page_addr = (desc & 0xff000000)
                    page_addr |= extract32(desc, 20, 4) << 32;
                    page_addr |= extract32(desc, 5, 4) << 36;
                    phys_addr = page_addr | (address & 0x00ffffff);
                    page_size = 0x1000000;
                    acc = desc & 0xffffff
                    if verbose:
                        print("Supersection", end=' ')
                        print(" -> 16M page at %X -> %X" % (page_addr, phys_addr))
                else:
                    page_addr = (desc & 0xfff00000)
                    page_size = 0x100000;
                    phys_addr = page_addr | (address & 0x000fffff);
                    acc = desc & 0xfffff
                    if verbose:
                        print("Section", end=' ')
                        print(" -> 1M page at %X -> %X" % (page_addr, phys_addr))
                ap = ((desc >> 10) & 3) | ((desc >> 13) & 4);
                texcb = ((desc >> 2) & 3) | (desc >> 10) & 0x1C
                xn = (desc >> 4) & 1
            else:
                if verbose:
                    print("Fault")
                continue

            yield address, phys_addr, page_size, ap, texcb, xn


def decode_ap(ap):
    if (ap == 0b001):
        return "P:RW"
    if (ap == 0b101):
        return "P:R "
    if (ap == 0b011):
        return "RW  "
    if verbose:
        print(bin(ap))
    assert False


def decode_cached_attr(x):
    if x == 0b00:
        #~ return "Non-cacheable"
        return "NCACH"
    if x == 0b01:
        #~ return "Write-Back, Write-Allocate"
        return "WB,WA"
    if x == 0b10:
        #~ return "Write-Through, no Write-Allocate"
        return "WT,WN"
    if x == 0b11:
        #~ return "Write-Back, no Write-Allocate"
        return "WB,WN"


def decode_texcb(texcb):
    # Notes on "texcb".
    #
    # "Bufferable (B), Cacheable (C), and Type Extension (TEX) bit names
    # are inherited from earlier versions of the architecture. These names
    # no longer adequately describe the function of the B, C, and TEX bits."
    #
    # Thanks, ARM manual.

    if texcb & 0b10000:
        return "O:%s I:%s " % (decode_cached_attr((texcb >> 2) & 3), decode_cached_attr(texcb & 3))
    if texcb == 0b00001:
        return "Device          "
    if texcb == 0b00000:
        return "Strongly-ordered"
    if verbose:
        print(bin(texcb))
    assert False


def decode_xn(xn):
    if xn:
        return "XN"
    return "  "


if __name__ == "__main__":
    main()

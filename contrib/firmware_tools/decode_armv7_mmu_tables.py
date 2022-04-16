#!/usr/bin/env python3

# Based on a1ex's v7mmap.py: https://a1ex.magiclantern.fm/bleeding-edge/R5/v7mmap.py
# Also see srsa's post about MMU: https://www.magiclantern.fm/forum/index.php?topic=25399.0
# Fundamentally based upon "ARM Architecture Reference Manual ARMv7-A and ARMv7-R edition"

import os
import sys
from struct import unpack

def getLongLE(d, address):
   return unpack('<L',d[address:address+4])[0]

def getByte(d, address):
   return ord(d[address])

def getString(d, address):
    return d[address : d.index('\0', address)]

def extract32(value, start, length):
    assert(start >= 0 and length > 0 and length <= 32 - start);
    return (value >> start) & (0xFFFFFFFF >> (32 - length));

ROM = open(sys.argv[1], "rb").read()
off = 0xE0000000

def scan(ttbrs):
    for i,ttbr in enumerate(ttbrs):
        print()
        print("TTBR%d: %08X" % (i, ttbr))
        print("===============")
        if not ttbr: continue
        for address in range(0, 1<<32, 1024):
            tbl = 1 if address >> (32-7) else 0
            if tbl != i: continue

            base_mask = 0xffffc000 if i == 1 else 0xffffff80
            entry_addr = (ttbr & base_mask) | ((address >> 18) & 0x3ffc)
            #~ print hex(ttbr), hex(ttbr & base_mask), hex((address >> 18) & 0x3ffc), hex(entry_addr)
            desc = getLongLE(ROM, entry_addr - off)
            type = desc & 3
            print(("%08X [%08X]: %08X %s" % (address, entry_addr, desc, bin(type|4)[3:])), end=' ')

            #~ print hex(address), hex(entry_addr), hex(desc), type
            if type == 1:
                domain = (desc >> 5) & 0x0f
                sbz = desc & 0x1E
                assert sbz == 0
                table_addr = (desc & 0xfffffc00)
                l2_entry = table_addr | ((address >> 10) & 0x3fc);
                desc = getLongLE(ROM, l2_entry - off)
                print("L2 table=%08X domain=%X entry=%X" % (table_addr, domain, l2_entry), end=' ')

                ap = ((desc >> 4) & 3) | ((desc >> 7) & 4);
                typ = (desc & 3)
                if typ == 0:
                    print(" -> fault")
                    continue
                elif typ == 1:
                    page_addr = (desc & 0xffff0000)
                    page_size = 0x10000;
                    phys_addr = page_addr | (address & 0xffff);
                    acc = desc & 0xffff
                    xn = desc & (1 << 15);
                    texcb = ((desc >> 2) & 3) | (desc >> 10) & 0x1C
                    print(" -> 64K page at %X -> %X" % (page_addr, phys_addr))
                elif typ == 2 or typ == 3:
                    page_addr = (desc & 0xfffff000)
                    page_size = 0x1000;
                    phys_addr = page_addr | (address & 0xfff);
                    acc = desc & 0xfff
                    xn = desc & 1;
                    texcb = ((desc >> 2) & 3) | (desc >> 4) & 0x1C
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
                    print("Supersection", end=' ')
                    print(" -> 16M page at %X -> %X" % (page_addr, phys_addr))
                else:
                    page_addr = (desc & 0xfff00000)
                    page_size = 0x100000;
                    phys_addr = page_addr | (address & 0x000fffff);
                    acc = desc & 0xfffff
                    print("Section", end=' ')
                    print(" -> 1M page at %X -> %X" % (page_addr, phys_addr))
                ap = ((desc >> 10) & 3) | ((desc >> 13) & 4);
                texcb = ((desc >> 2) & 3) | (desc >> 10) & 0x1C
                xn = (desc >> 4) & 1
            else:
                print("Fault")
                continue

            #~ print >> sys.stderr, hex(address), hex(phys_addr)
            yield address, phys_addr, page_size, ap, texcb, xn

last_offset = None
vstart = 0
pstart = 0
size = 0
last_ap = None
last_xn = None
last_texcb = None

def decode_ap(ap):
    if (ap == 0b001):
        return "P:RW"
    if (ap == 0b101):
        return "P:R "
    if (ap == 0b011):
        return "RW  "
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
    if texcb  & 0b10000:
        return "O:%s I:%s " % (decode_cached_attr((texcb >> 2) & 3), decode_cached_attr(texcb & 3))
    if texcb == 0b00001:
        return "Device          "
    if texcb == 0b00000:
        return "Strongly-ordered"
    print(bin(texcb))
    assert False

def decode_xn(xn):
    if xn:
        return "XN"
    return "  "

for a, p, s, ap, texcb, xn in scan([0xE0004800, 0xE0000000]):  # R5 core 0 (0xDFFC4800 / 0xDFFC0000, copied from 0xE0000000)
#~ for a, p, s, ap, texcb, xn in scan([0xE0004880, 0xE0000000]):  # R5 core 1 (0xDFFC4880 / 0xDFFC0000, copied from 0xE0000000)
#~ for a, p, s, ap, texcb, xn in scan([0xE0004800, 0xE0000080]):  # M50 core 0
#~ for a, p, s, ap, texcb, xn in scan([0xE0004880, 0xE0000080]):  # M50 core 1
    offset = p - a
    s = 1024
    if offset == last_offset and ap == last_ap and xn == last_xn and texcb == last_texcb and a == vstart + size:
        size += s
    else:
        if last_offset is not None:
            print("%08X-%08X -> %08X-%08X (%+5X) %-4s %s %s" % (vstart, vstart+size-1, pstart, pstart+size-1, last_offset, decode_texcb(last_texcb), decode_ap(last_ap), decode_xn(last_xn)), file=sys.stderr)
        vstart, pstart, size = a, p, s
        last_offset = offset
        last_ap = ap
        last_xn = xn
        last_texcb = texcb

    #~ print "%08X-%08X: %08X-%08X (tmp)" % (vstart, vstart+size, pstart, pstart+size)

print("%08X-%08X -> %08X-%08X (%+5X) %-4s %s %s" % (vstart, vstart+size-1, pstart, pstart+size-1, last_offset, decode_texcb(last_texcb), decode_ap(last_ap), decode_xn(last_xn)), file=sys.stderr)


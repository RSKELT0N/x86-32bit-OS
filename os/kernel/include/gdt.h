#ifndef _GDT_H
#define _GDT_H

#include <kernel/include/gdt_info.h>
#include <stdlibc/include/string.h>
#include <stdlibc/include/stdint.h>

namespace kernel {

    struct GDT_table {
            uint16_t gdt_size;          // Size of the GDT.
            uint32_t gdt_entry_addr;    // Start address of the GDT.
    } __attribute__((packed));

    struct SegmentDescriptor {
        uint16_t limit_lo; //  A 20-bit value, tells the maximum addressable unit.
        uint16_t base_lo;  // A 32-bit value containing the linear address where the segment begins.
        uint8_t base_hi;   // middle 8 bit base value.

        uint8_t AC  : 1;   // Accessed bit.
        uint8_t RW  : 1;   // Readable | Writable bit.
        uint8_t DC  : 1;   // Direction | conforming bit.
        uint8_t E   : 1;   // Executable bit.
        uint8_t S   : 1;   // Descriptor bit.
        uint8_t DPL : 2;   // Descriptor privilege field.
        uint8_t P   : 1;   // Present bit.

        uint8_t limit_hi : 4;   // high 4 bits for limit value
        uint8_t L        : 1;   // Long mode,
        uint8_t reserved : 1;   // Reserved bit, unused.
        uint8_t DB       : 1;   // Size flag, what gdt bit mode (16/32).
        uint8_t G        : 1;   // Granularity flag.

        uint8_t base_vhi : 8;   // high 8 bit base value.
    } __attribute__((packed));

    extern "C" void _load_GDT();
    void _init_GDT() noexcept;
    void define_gdt_segment(SegmentDescriptor* entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags) noexcept;

};

#endif // _GDT_H
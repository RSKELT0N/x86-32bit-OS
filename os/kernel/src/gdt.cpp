#include <kernel/include/gdt.h>
#include <kernel/include/screen.h>

using namespace kernel;

struct GDT_table _gdt_table;
struct SegmentDescriptor m_gdt_entries[GDT_SEGMENTS_C];

void kernel::_init_GDT() noexcept {
    memset(static_cast<void*>(&m_gdt_entries), 0, sizeof(m_gdt_entries));
    memset(static_cast<void*>(&_gdt_table), 0, sizeof(_gdt_table));

    define_gdt_segment(&m_gdt_entries[GDT_SEGMENT_NULL_INDEX], 0x0, 0, 0, 0);
    define_gdt_segment(&m_gdt_entries[GDT_SEGMENT_KERNEL_CODE_INDEX], 0x0, 0xFFFFF, GDT_CODE_PL0, SEG_FLAGS);
    define_gdt_segment(&m_gdt_entries[GDT_SEGMENT_KERNEL_DATA_INDEX], 0x0, 0xFFFFF, GDT_DATA_PL0, SEG_FLAGS);
    define_gdt_segment(&m_gdt_entries[GDT_SEGMENT_USER_CODE_INDEX],   0x0, 0xFFFFF, GDT_CODE_PL3, SEG_FLAGS);
    define_gdt_segment(&m_gdt_entries[GDT_SEGMENT_USER_DATA_INDEX],   0x0, 0xFFFFF, GDT_DATA_PL3, SEG_FLAGS);

    _gdt_table.gdt_size = sizeof(m_gdt_entries);
    _gdt_table.gdt_entry_addr = reinterpret_cast<uint32_t>(&m_gdt_entries);

    _load_GDT();
    terminal_writestring("-> GDT initialised\n");
}

void kernel::define_gdt_segment(SegmentDescriptor* entry,
     uint32_t base,
     uint32_t limit,
     uint8_t access, uint8_t flags) noexcept
{

    entry->base_lo  = (base & 0x0000FFFF);
    entry->base_hi  = (base & 0x00FF0000) >> 16;
    entry->base_vhi = (base & 0xFF000000) >> 24;

    entry->limit_lo = (limit & 0x0000FFFF);
    entry->limit_hi = (limit & 0x000F0000) >> 16;

    entry->AC  = (access & SEG_ACCESS_ACCESS);
    entry->RW  = (access & SEG_ACCESS_RW)      >> 1;
    entry->DC  = (access & SEG_ACCESS_DC)      >> 2;
    entry->E   = (access & SEG_ACCESS_EXE)     >> 3;
    entry->S   = (access & SEG_ACCESS_TYPE)    >> 4;
    entry->DPL = (access & 0b01100000)         >> 5;
    entry->P   = (access & SEG_ACCESS_PRESENT) >> 7;

    entry->DB = (flags & 0b1000) >> 3;
    entry->G  = (flags & 0b0100) >> 2;

    entry->L  = 0;        // Long mode, default 0
    entry->reserved = 0;  // Intel reserved bit, default 0
}

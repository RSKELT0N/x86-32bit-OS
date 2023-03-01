#include <kernel/include/idt.h>

using namespace kernel;

extern "C" void _load_IDT();
extern "C" void _KB_handler();
struct IDT_Descriptor_t _idt_table;
struct GateDescriptor_t m_idt_entries[IDT_GATES_C];

void kernel::_init_IDT() noexcept {
	for(uint32_t i = 1; i < IDT_GATES_C; i++) {
		define_idt_segment(&m_idt_entries[i], 0, 0, IDT_EMPTY_GATE);
	}

	define_idt_segment(&m_idt_entries[DRIVER_KB_OFFSET_IRQ], GDT_SEGMENT_KERNEL_CODE_ADDR, reinterpret_cast<uint32_t>(&_KB_handler), IDT_INTERRUPT_TYPE_PL0);

	_idt_table.idt_size = sizeof(m_idt_entries);
	_idt_table.idt_addr = (reinterpret_cast<uint32_t>(&m_idt_entries));
	_load_IDT();
    terminal_writestring("-> IDT initialised\n");
}

void kernel::define_idt_segment(GateDescriptor_t* entry, uint16_t seg_selector, uint32_t offset, uint8_t type) noexcept {
	entry->offset_lo = (offset & 0x0000FFFF);
	entry->offset_hi = (offset & 0xFFFF0000) >> 16;

	entry->seg_selector = seg_selector;
	entry->type_attributes = type;
	entry->reserved = 0;
}

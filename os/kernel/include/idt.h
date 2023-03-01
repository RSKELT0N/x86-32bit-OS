#ifndef _IDT_H
#define _IDT_H

#include <kernel/include/idt_info.h>
#include <kernel/include/gdt_info.h>
#include <stdlibc/include/stdint.h>
#include <kernel/include/screen.h>
#include <kernel/include/pic.h>

#include <kernel/include/keyboard.h>

namespace kernel {

	struct GateDescriptor_t {
		uint16_t offset_lo;		    // Low 16 bit value of the address point for ISR.
		uint16_t seg_selector;	    // Offset value selector from the GDT. To be used as base address for the offset.
		uint8_t reserved;		    // Reserved bits by Intel.
		uint8_t type_attributes;	// Defines the type of the gate this Interrupt descriptor represents.
									/*  - Contains
										: 4 bit value for the gate type.
										: 2 bit value for the DPL for the CPU.
										: 1 bit value for the gate to be present.
										: 1 bit value, reserved. Default 0.
									*/
		uint16_t offset_hi;			// High 16 bit value of the address point for ISR.
	} __attribute__((packed));

	struct IDT_Descriptor_t {
		uint16_t idt_size;		    // Size of the IDT table.
		uint32_t idt_addr;		    // 32 bit address which points to the start of the IDT table.
	} __attribute__((packed));

	void _init_IDT() noexcept;
	void define_idt_segment(GateDescriptor_t* entry, uint16_t seg_selector, uint32_t offset, uint8_t type) noexcept;

};

#endif // _IDT_H
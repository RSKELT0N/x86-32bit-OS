#ifndef _IDT_INFO_H
#define _IDT_INFO_H

#include <stdlibc/include/stdint.h>

namespace kernel {

	// IDT INFO
	const uint32_t IDT_GATES_C = 255;
	const uint8_t IDT_RING0 = 0b00000000;
	const uint8_t IDT_RING3 = 0b01100000;
	const uint8_t IDT_PRESENT = 0b10000000;

	// Gate Types
	const uint8_t IDT_TRAP32_GATE = 0xF;
	const uint8_t IDT_TASK32_GATE = 0x5;
	const uint8_t IDT_INTERRUPT32_GATE = 0xE;

	// EMPTY GATE
	const uint8_t IDT_EMPTY_GATE          = IDT_RING0 |
								            IDT_TRAP32_GATE;

	// TYPE ATTRIBUTES - RING0
	const uint8_t IDT_TRAP_TYPE_PL0       = IDT_PRESENT |
									        IDT_RING0   |
									        IDT_TRAP32_GATE;

	const uint8_t IDT_TASK_TYPE_PL0       = IDT_PRESENT |
									        IDT_RING0   |
									        IDT_TASK32_GATE;

	const uint8_t IDT_INTERRUPT_TYPE_PL0  = IDT_PRESENT |
										    IDT_RING0   |
										    IDT_INTERRUPT32_GATE;

	// TYPE ATTRIBUTES - RING3
	const uint8_t IDT_TRAP_TYPE_PL3       = IDT_PRESENT |
									        IDT_RING3    |
									        IDT_TRAP32_GATE;

	const uint8_t IDT_TASK_TYPE_PL3       = IDT_PRESENT |
									        IDT_RING3   |
									        IDT_TASK32_GATE;

	const uint8_t IDT_INTERRUPT_TYPE_PL3  = IDT_PRESENT |
										    IDT_RING3   |
										    IDT_INTERRUPT32_GATE;

};


#endif // _IDT_INFO_H
#ifndef _PIC_H
#define _PIC_H

#include <kernel/include/port.h>
#include <kernel/include/screen.h>
#include <stdlibc/include/stdint.h>

namespace kernel {

	// I/0 Ports for Master and slave PIC.
	#define PIC1_COMMAND		0x20		// PIC1 command port.
	#define PIC1_DATA	    	0x21		// PIC1 data port.
	#define PIC2_COMMAND		0xA0		// PIC2 command port.
	#define PIC2_DATA	    	0xA1		// PIC2 data port.
	#define PIC_EOI		    	0x20		/* End-of-interrupt command code */
	#define PIC_FMASK			0xFF		// Full mask for the PIC.

	// Offset vectors for PIC/
	#define PIC1_OFFSET_START	0x20		// PIC1 start offset.
	#define PIC1_OFFSET_END		0x27		// PIC1 end offset.
	#define PIC2_OFFSET_START	0x28		// PIC2 start offset.
	#define PIC2_OFFSET_END		0x2F		// PIC2 end offset.

	// Command code to set up the PIC offset.
	#define ICW1_ICW4			0x01		/* ICW4 (not) needed */
	#define ICW1_SINGLE			0x02		/* Single (cascade) mode */
	#define ICW1_INTERVAL4		0x04		/* Call address interval 4 (8) */
	#define ICW1_LEVEL			0x08		/* Level triggered (edge) mode */
	#define ICW1_INIT			0x10		/* Initialization - required! */
	 
	#define ICW4_8086	    	0x01		/* 8086/88 (MCS-80/85) mode */
	#define ICW4_AUTO	    	0x02		/* Auto (normal) EOI */
	#define ICW4_BUF_SLAVE		0x08		/* Buffered mode/slave */
	#define ICW4_BUF_MASTER		0x0C		/* Buffered mode/master */
	#define ICW4_SFNM	    	0x10		/* Special fully nested (not) */

	// Unsed port for waiting intervals between port interactions.
	#define UNUSED_PORT 		0x80		// to wait between port communication.

	void _io_wait();
	void _sendEOI_PIC(uint8_t irq);
	void _remap_PIC();
	void _IRQ_set_mask(uint8_t IRQline);
	void _IRQ_clear_mask(uint8_t IRQline);
	uint8_t _getIRQ_from_offset(uint8_t offset);
	uint8_t _getOffset_from_IRQ(uint8_t IRQ);

};

#endif // _PIC_H
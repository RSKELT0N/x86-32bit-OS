#include <kernel/include/pic.h>

static kernel::Port _port(kernel::Port::BitType::_8Bit);

void kernel::_io_wait() {
	_port.write(UNUSED_PORT, 0);
}

void kernel::_sendEOI_PIC(uint8_t irq) {
	if(irq > 7)
		_port.write(PIC2_COMMAND, PIC_EOI);

	_port.write(PIC1_COMMAND, PIC_EOI);
}

 /* These bytes give the PIC:

    Its vector offset. (ICW2)
    Tell it how it is wired to master/slaves. (ICW3)
    Gives additional information about the environment. (ICW4) 
 */
void kernel::_remap_PIC() {	

	_port.write(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	kernel::_io_wait();
	_port.write(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	kernel::_io_wait();
	_port.write(PIC1_DATA, PIC1_OFFSET_START);                 // ICW2: Master PIC vector offset
	kernel::_io_wait();
	_port.write(PIC2_DATA, PIC2_OFFSET_START);                 // ICW2: Slave PIC vector offset
	kernel::_io_wait();
	_port.write(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	kernel::_io_wait();
	_port.write(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	kernel::_io_wait();
 
	_port.write(PIC1_DATA, ICW4_8086);
	kernel::_io_wait();
	_port.write(PIC2_DATA, ICW4_8086);
	kernel::_io_wait();
 
	_port.write(PIC1_DATA, PIC_FMASK);   // masks PIC1 to 0xFF.
	_port.write(PIC2_DATA, PIC_FMASK);	 // masks PIC2 to 0xFF.

    kernel::terminal_writestring("-> PIC initialised\n");
}

void kernel::_IRQ_set_mask(uint8_t IRQline) {
	uint8_t data;
	uint16_t port = (IRQline < 8) ? PIC1_DATA : PIC2_DATA;
	IRQline = (port == PIC2_DATA) ? IRQline - 8 : IRQline;

	data = _port.read(port) | (1 << IRQline);
	_port.write(port, data);
}

void kernel::_IRQ_clear_mask(uint8_t IRQline) {
	uint8_t data;
	uint16_t port = (IRQline < 8) ? PIC1_DATA : PIC2_DATA;
	IRQline = (port == PIC2_DATA) ? IRQline - 8 : IRQline;

	data = _port.read(port) & ~(1 << IRQline);	
	_port.write(port, data);
}

uint8_t kernel::_getIRQ_from_offset(uint8_t offset) {
	if(offset < PIC2_OFFSET_START) {
		offset -= PIC1_OFFSET_START;
	} else {
		offset -= PIC2_OFFSET_START;
	}
	return offset;
}

uint8_t kernel::_getOffset_from_IRQ(uint8_t IRQ) {
	if(IRQ > 7) {
		IRQ += PIC2_OFFSET_START; 
	} else {
		IRQ += PIC1_OFFSET_START;
	}
	return IRQ;
}
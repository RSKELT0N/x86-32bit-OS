#include <kernel/include/keyboard.h>

using namespace kernel;

static Port _port(Port::BitType::_8Bit);

void kernel::_init_keyboard() noexcept {
	_IRQ_clear_mask(DRIVER_KB_IRQ);
	terminal_writestring("-> Keyboard initialised\n");
}

void kernel::_handle_KB_interrupt() {
	uint8_t scan_code = _port.read(DRIVER_KB_DATA_PORT);
	terminal_writebin(scan_code, 8);
	_sendEOI_PIC(DRIVER_KB_IRQ);
}
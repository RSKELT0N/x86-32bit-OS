#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <kernel/include/screen.h>
#include <kernel/include/pic.h>
#include <kernel/include/port.h>
#include <stdlibc/include/stdint.h>

namespace kernel {

	const uint8_t DRIVER_KB_IRQ        = 0x1;
	const uint8_t DRIVER_KB_OFFSET_IRQ = _getOffset_from_IRQ(DRIVER_KB_IRQ);
	const uint16_t DRIVER_KB_DATA_PORT = 0x60;

	void _init_keyboard() noexcept;
	extern "C" void _handle_KB_interrupt();

};

#endif // _KEYBOARD_H
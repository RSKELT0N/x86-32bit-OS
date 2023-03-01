#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <kernel/include/vga.h>
#include <stdlibc/include/stdint.h>
#include <stdlibc/include/string.h>

namespace kernel {

	void terminal_setcolor(uint8_t color);
	void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
	void terminal_putchar(char c);
	void terminal_write(const char* data, size_t size);
	void terminal_writestring(const char* data);
	void terminal_writeval(uint32_t val);
	void terminal_writebin(uint32_t val, int8_t size);

};

#endif // _SCREEN_H
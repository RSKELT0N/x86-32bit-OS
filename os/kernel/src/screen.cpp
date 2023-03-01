#include <kernel/include/screen.h>

extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;
extern size_t terminal_row;
extern size_t terminal_column;

void kernel::terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void kernel::terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * kernel::VGA_WIDTH + x;
    terminal_buffer[index] = kernel::vga_entry(c, color);
}

void kernel::terminal_putchar(char c) {
    if (++terminal_column == kernel::VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == kernel::VGA_HEIGHT)
            terminal_row = 0;
    }

    switch(c) {
        case '\n': terminal_column = 0;
            terminal_row++;
            break;
        case '\r':  terminal_column = 0;
            break;
        default:
            terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
            break;
    }
}

void kernel::terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void kernel::terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void kernel::terminal_writeval(uint32_t val) {
    char* vals = (char *)"";
    itoa(val, vals, 10);
    terminal_writestring(vals);
}

void kernel::terminal_writebin(uint32_t val, int8_t size) {
    for(int i = size - 1; i >= 0; i--) {
        uint32_t val_ = (val >> i) & 1;
        terminal_writeval(val_);
    }
    terminal_writestring("\n");
}

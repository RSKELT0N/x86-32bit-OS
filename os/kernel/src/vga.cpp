#include <kernel/include/vga.h>

using namespace kernel;

uint8_t terminal_color;
uint16_t* terminal_buffer;

size_t terminal_row = 0;
size_t terminal_column = 0;

static Port _port(Port::BitType::_8Bit);

void kernel::_enable_cursor(uint8_t start_pos, uint8_t end_pos) {
    _port.write(VGA_CRTC_CONTROL_PORT, VGA_CURSOR_START_REG);
    _port.write(VGA_CRTC_DATA_PORT, (_port.read(VGA_CRTC_DATA_PORT) & 0xC0) | start_pos);

    _port.write(VGA_CRTC_CONTROL_PORT, VGA_CURSOR_END_REG);
    _port.write(VGA_CRTC_DATA_PORT, (_port.read(VGA_CRTC_DATA_PORT) & 0xE0) | end_pos);

    _set_cursor(terminal_row, terminal_column);
}

void kernel::_disable_cursor() {
    _port.write(VGA_CRTC_CONTROL_PORT, VGA_CURSOR_START_REG);
    _port.write(VGA_CRTC_DATA_PORT, 0x20);
}

void kernel::_set_cursor(uint16_t x, uint16_t y) {
    uint16_t pos = y * VGA_WIDTH + x;

    _port.write(VGA_CRTC_CONTROL_PORT, VGA_CONTROL_CURSOR_HIGH_REG);
    _port.write(VGA_CRTC_DATA_PORT, (uint8_t) ((pos >> 8) & 0xFF));
    
    _port.write(VGA_CRTC_CONTROL_PORT, VGA_CONTROL_CURSOR_LOW_REG);
    _port.write(VGA_CRTC_DATA_PORT, (uint8_t) (pos & 0xFF));
}

_cursor_pos_t kernel::_get_cursor() {
    _cursor_pos_t cursor_pos;
    uint16_t pos;

    _port.write(VGA_CRTC_CONTROL_PORT, VGA_CONTROL_CURSOR_LOW_REG);
    pos = _port.read(VGA_CRTC_DATA_PORT);

    _port.write(VGA_CRTC_CONTROL_PORT, VGA_CONTROL_CURSOR_HIGH_REG);
    pos |= (uint16_t) _port.read(VGA_CRTC_DATA_PORT) << 8;

    cursor_pos.x = (size_t) pos % VGA_WIDTH;
    cursor_pos.y = (size_t) pos / VGA_WIDTH;

    return cursor_pos;
}

void kernel::_init_VGA(void) {
    kernel::_enable_cursor(0, VGA_WIDTH);

    terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) VGA_MEMORY_ADDRESS;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_writestring("-> VGA initialised\n");
}


uint8_t kernel::vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t kernel::vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) color << 8 | (uint16_t) uc;
}


#ifndef _VGA_H
#define _VGA_H

#include <stdlibc/include/stdint.h>
#include <kernel/include/port.h>
#include <kernel/include/screen.h>

namespace kernel {

    constexpr size_t   VGA_MEMORY_ADDRESS = 0xB8000;

    constexpr uint8_t  VGA_WIDTH = 80;
    constexpr uint8_t  VGA_HEIGHT = 25;

    constexpr size_t   VGA_START_POS_X = 0; 
    constexpr size_t   VGA_START_POS_Y = 0; 

    constexpr uint16_t VGA_CRTC_CONTROL_PORT = 0x3D4;
    constexpr uint16_t VGA_CRTC_DATA_PORT = 0x3D5;

    constexpr uint8_t  VGA_CURSOR_START_REG = 0x0A;
    constexpr uint8_t  VGA_CURSOR_END_REG = 0x0B;
    constexpr uint8_t  VGA_CONTROL_CURSOR_HIGH_REG = 0xE;
    constexpr uint8_t  VGA_CONTROL_CURSOR_LOW_REG = 0xF;

    struct _cursor_pos_t {
        size_t x;
        size_t y;
    } __attribute__((packed));

    /* Hardware text mode color constants. */
    enum vga_color {
        VGA_COLOR_BLACK         = 0,
        VGA_COLOR_BiUE          = 1,
        VGA_COLOR_GREEN         = 2,
        VGA_COLOR_CYAN          = 3,
        VGA_COLOR_RED           = 4,
        VGA_COLOR_MAGENTA       = 5,
        VGA_COLOR_BROWN         = 6,
        VGA_COLOR_LIGHT_GREY    = 7,
        VGA_COLOR_DARK_GREY     = 8,
        VGA_COLOR_LIGHT_BLUE    = 9,
        VGA_COLOR_LIGHT_GREEN   = 10,
        VGA_COLOR_LIGHT_CYAN    = 11,
        VGA_COLOR_LIGHT_RED     = 12,
        VGA_COLOR_LIGHT_MAGENTA = 13,
        VGA_COLOR_LIGHT_BROWN   = 14,
        VGA_COLOR_WHITE         = 15,
    };


    void _disable_cursor();
    _cursor_pos_t _get_cursor();
    void _set_cursor(uint16_t x, uint16_t y);
    void _enable_cursor(uint8_t start_pos, uint8_t end_pos);

    void _init_VGA();
    uint16_t vga_entry(unsigned char uc, uint8_t color);
    uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);

};

#endif // _VGA_H
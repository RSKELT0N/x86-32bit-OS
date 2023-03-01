#ifndef _GDT_INFO_H
#define _GDT_INFO_H

#include <stdlibc/include/stdint.h>

namespace kernel {

    const uint8_t GDT_SEGMENTS_C                = 5;
    const uint8_t GDT_SEGMENT_NULL_INDEX        = 0;
    const uint8_t GDT_SEGMENT_KERNEL_CODE_INDEX = 1;
    const uint8_t GDT_SEGMENT_KERNEL_DATA_INDEX	= 2;
    const uint8_t GDT_SEGMENT_USER_CODE_INDEX   = 3;
    const uint8_t GDT_SEGMENT_USER_DATA_INDEX   = 4;

    const uint16_t GDT_SEGMENT_KERNEL_CODE_ADDR = 0x08;
    const uint16_t GDT_SEGMENT_KERNEL_DATA_ADDR = 0x10;


    const uint8_t SEG_ACCESS_PRESENT    = 0b10000000;
    const uint8_t SEG_ACCESS_RING0      = 0b00000000;
    const uint8_t SEG_ACCESS_RING3      = 0b01100000;
    const uint8_t SEG_ACCESS_TYPE       = 0b00010000;
    const uint8_t SEG_ACCESS_EXE        = 0b00001000;
    const uint8_t SEG_ACCESS_DC         = 0b00000100;
    const uint8_t SEG_ACCESS_RW         = 0b00000010;
    const uint8_t SEG_ACCESS_ACCESS     = 0b00000001;
    const uint8_t SEG_FLAGS             = 0xc;

    const uint8_t GDT_CODE_PL0  = SEG_ACCESS_TYPE   |
                                  SEG_ACCESS_EXE    |
                                  SEG_ACCESS_RW     |
                                  SEG_ACCESS_RING0  |
                                  SEG_ACCESS_PRESENT;

    const uint8_t GDT_DATA_PL0 = SEG_ACCESS_TYPE   |
                                 SEG_ACCESS_RW     |
                                 SEG_ACCESS_RING0  |
                                 SEG_ACCESS_PRESENT;

    const uint8_t GDT_CODE_PL3 = SEG_ACCESS_TYPE   |
                                 SEG_ACCESS_EXE    |
                                 SEG_ACCESS_RW     |
                                 SEG_ACCESS_RING3  |
                                 SEG_ACCESS_PRESENT;

    const uint8_t GDT_DATA_PL3 = SEG_ACCESS_TYPE   |
                                 SEG_ACCESS_RW     |
                                 SEG_ACCESS_RING3  |
                                 SEG_ACCESS_PRESENT;

};

#endif // _GDT_INFO_H

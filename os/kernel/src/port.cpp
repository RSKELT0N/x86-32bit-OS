#include <kernel/include/port.h>

using namespace kernel;

kernel::Port::Port(Port::BitType bit) {
    this->m_bit = bit;
}

uint32_t kernel::Port::read(uint16_t port) const noexcept {
    uint32_t result;

    switch(m_bit) {
        case BitType::_16Bit: result = (uint16_t)_16Bit_read(port); break;
        case BitType::_32Bit: result = _32Bit_read(port);           break;
        default: result              = (uint8_t)_8Bit_read(port);   break;
    }
    return result;
}

void kernel::Port::write(uint16_t port, uint32_t data) const noexcept {
    switch(m_bit) {
        case BitType::_8Bit:     _8Bit_write(port,     (uint8_t)data);  break;
        case BitType::_8BitSlow: _8BitSlow_write(port, (uint8_t)data);  break;
        case BitType::_16Bit:    _16Bit_write(port,    (uint16_t)data); break;
        case BitType::_32Bit:    _32Bit_write(port,    (uint32_t)data); break;
    }
}

uint8_t kernel::Port::_8Bit_read(uint16_t port) const noexcept {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

uint16_t kernel::Port::_16Bit_read(uint16_t port) const noexcept {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

uint32_t kernel::Port::_32Bit_read(uint16_t port) const noexcept {
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void kernel::Port::_8Bit_write(uint16_t port, uint8_t data) const noexcept {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (port));
}

void kernel::Port::_8BitSlow_write(uint16_t port, uint8_t data) const noexcept {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (port));

}

void kernel::Port::_16Bit_write(uint16_t port, uint16_t data) const noexcept {
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (port));
}

void kernel::Port::_32Bit_write(uint16_t port, uint32_t data) const noexcept {
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (port));
}


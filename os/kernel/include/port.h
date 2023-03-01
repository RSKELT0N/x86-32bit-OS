#ifndef _PORT_H
#define _PORT_H

#include <stdlibc/include/stdint.h>

namespace kernel {

    class Port {
    public:
        enum BitType {
            _8Bit,
            _8BitSlow,
            _16Bit,
            _32Bit
        };

    public:
        Port(BitType bit);
        ~Port() = default;

    public:
        uint32_t read(uint16_t port) const noexcept;
        void write(uint16_t port, uint32_t data) const noexcept;

    private:
        uint8_t _8Bit_read(uint16_t port) const noexcept;
        uint16_t _16Bit_read(uint16_t port) const noexcept;
        uint32_t _32Bit_read(uint16_t port) const noexcept;
        void _8Bit_write(uint16_t port, uint8_t data) const noexcept;
        void _8BitSlow_write(uint16_t port, uint8_t data) const noexcept;
        void _16Bit_write(uint16_t port, uint16_t data) const noexcept;
        void _32Bit_write(uint16_t port, uint32_t data) const noexcept;

    private:
        Port::BitType m_bit;
    };

};

#endif // _PORT_H
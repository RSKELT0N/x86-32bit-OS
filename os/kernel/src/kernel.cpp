#include <kernel/include/gdt.h>
#include <kernel/include/idt.h>
#include <kernel/include/screen.h>
#include <kernel/include/pic.h>
#include <kernel/include/keyboard.h>

// Global constructors
extern "C" void (*_start_ctors)();
extern "C" void (*_end_ctors)();

// Global destructors
extern "C" void (*_start_dtors)();
extern "C" void (*_end_dtors)();

// .bss addresses
extern "C" int _bss_start;
extern "C" int _bss_end;

void _init_bss() {
    for(int* i = (int*)_bss_start; i < (int*)_bss_end; i++) {
        *i = 0;
    }
 }

void _call_constructors() {
	for(void (**i)() = &_start_ctors; *i != _end_ctors; i++) {
		(*i)();
	}
}

void _call_destructors() {
    for(void (**i)() = &_start_dtors; *i != _end_dtors; i++) {
        (*i)();
    }
}

extern "C" void kernel_early_main() {
    _init_bss();
    _call_constructors();

    kernel::_init_VGA();
    kernel::_init_GDT();
    kernel::_init_IDT();
    kernel::_remap_PIC();
    kernel::_init_keyboard();
}

extern "C" void kernel_close() {
    _call_destructors();
}

extern "C" void kernel_main(const void* multiboot_structure, uint32_t magicnumber) {
    while(1); // Halt the OS from returning from kernel.
}

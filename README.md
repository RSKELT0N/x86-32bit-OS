# x86_32bit-OS
Attempting to create a 32-bit x86 operating system, that performs as a minimal system.

## Requirements
- Makefile expects i686-elf-g++ compiler and i686-elf-ld linker.
    - Can use 'make build-cross-compiler' to build i686 toolchain, set paths and update makefile to pick this up.
    - Can install a i686 toolchain 
        - With brew: brew install i686-elf-gcc
 - cdrtools

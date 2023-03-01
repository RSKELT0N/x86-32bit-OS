/*
=====================================================================
#			      			Linker file
#   
#      - Groups different compiled languages under a structure, and
#             to be understood by the booloader.
=====================================================================
*/


/*
- (ENTRY(_start)): This indicates the entry point of our OS, which
will jump to '_start' in 'loader.asm', once the image is
loaded into memory.

- (OUTPUT_FORMAT(elf32-i386)): Tells the linker which format output
to select for the executable.

- (OUTPUT_ARCH(i386:i386)): Tells the linker which output architecture set,
will the executable use. To suit the proposed CPU.
*/
ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)
 
/*
 - Tells where the various sections of the object files will be put in the final
kernel image. 
 */
SECTIONS
{
	/* 
	- Begin putting sections at 1 MiB, a conventional place for kernels to be
	loaded at by the bootloader.
	*/
	. = 0x00100000;
 
 	/*
 	- Insert (.multiboot) section as start of (.text) section. Allows
 	the bootloader to recognise our program as a kernel, for it to be
 	bootable and passes control to us.

 	- Following, is (.text) section. This is our code of the operating
 	system/ kernel.
 	*/
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
 
	/* Read-only data. */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		_start_ctors = .;
		KEEP(*( .init_array ));
		KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
		_end_ctors = .;

		_start_dtors = .;
        KEEP(*( .fini_array ));
        KEEP(*(SORT_BY_INIT_PRIORITY( .fini_array.* )));
        _end_dtors = .;
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
	    _bss_start = .;
		*(COMMON)
		*(.bss)
	    _bss_end = .;
	}

	/*
	- Discarding left over symbols, at the end of generating the image.
	These symbols, can be removed from the final image of the kernel.
	*/
	/DISCARD/ :
	{
		*(.comment)
	}

}

/* EOF */

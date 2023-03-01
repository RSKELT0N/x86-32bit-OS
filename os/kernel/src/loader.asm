;====================================================================
;			   		 	Loader file
;
;	  - Assembly code, which defines multiboot include for
;  bootloader to recognise the program as a kernel.
;
;  - Creates .multiboot section for bootloader to see magic
;  values to recognise this. Entry point (_start), initialises
;  top of kernel stack (setting %esp register, needed for c++) and
;	pushes MAGIC value (0X1BADB002), multi-structure onto stack.
;
;  - Kernel loader start at entry point (_start), will call
;   function in c++ (kernel_main) to transition from asm to c++.
;   		       (makes life easier)
;
;  The bootloader has loaded us into 32-bit protected mode on a x86
;  machine. Interrupts are disabled. Paging is disabled. The processor
;  state is as defined in the multiboot standard. The kernel has full
;  control of the CPU. The kernel can only make use of hardware features
;  and any code it provides as part of itself.
;====================================================================

use32

MAGIC        	  	equ 0x1BADB002        ; 'magic number' lets bootloader find the include.
FLAGS             	equ (1<<0 | 1<<1)     ; this is the Multiboot 'flag' field.
CHECKSUM          	equ -(MAGIC + FLAGS)  ; checksum of above, to prove we are multiboot.
KERNEL_STACK_SIZE 	equ (16 * 1024)       ; Amount of bytes to skip to set the stack register.

MB_MAGIC_ADDR       equ 0x90000           ; address of the magic number to be passed into c++ kernel (by default it's put into EAX)
MB_STRUCT_ADDR      equ 0x90004           ; address of the multiboot structure to be passed into c++ kernel

section .multiboot
align 4
	dd	MAGIC
	dd	FLAGS
	dd	CHECKSUM


extern kernel_early_main
extern kernel_main
extern kernel_close

section .text		; defining the .text section.
global _start		; make _start visibile to outside files.
_start:
    ; Transferring registers for initialising, Storing multi boot valuee and struct.
    mov     ecx, MB_MAGIC_ADDR		; Moving the magic value address to ecx.
    mov     dword [ecx], eax		; Moving the value at eax towards the address of ecx.

    mov     eax, MB_STRUCT_ADDR		; Moving the struct address into reg eax.
    mov     dword [eax], ebx		; Moving the value at ebx towards the address of eax

	mov 	esp, kernel_stack_bottom + KERNEL_STACK_SIZE	; Moving the top stack address towards the esp register, for c calls.
	call    kernel_early_main								; Call the external linkage c function, to initialise early kernel.

    mov     eax, MB_MAGIC_ADDR		; Moving the magic address back towards eax.
    mov     dword ebx, [eax]		; Moving the value of eax into ebx.

    mov     ecx, MB_STRUCT_ADDR		; Moving the multiboot struct address towards ecx.
    mov     dword eax, [ecx]		; Moving the value, stored at ecx. Towards eax.

	push	eax                     ; Pushing the multiboot magic value. To be retrieved in C.				        
	push	ebx				        ; Pushing the multiboot structure onto stack.
	call	kernel_main		        ; Calling kernel function within Cpp, to change language.
	call    kernel_close	        ; Calling the external function, to de-initialise the kernel.

    cli 							; Disable interrupts.
_stop:
	hlt 							; Halt the cpu
	jmp 	_stop

section .bss
align 16
kernel_stack_bottom:
	resb 	KERNEL_STACK_SIZE ; 16 KiB
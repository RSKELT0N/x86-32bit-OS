use32                       ; Tell the compiler to use 32 bit (Protected mode).

section .text

extern _gdt_table           ; Address of the GDT table.
extern _idt_table           ; Address of the IDT table

[global _load_GDT]
_load_GDT:
    lgdt    [_gdt_table]    ; Load the GDT into GDTR.
    mov     ax, 0x10        ; Move the address of the Kdata segment into ax.
    mov     ds, ax          ; Move the data segment address into ds, this will update the segment registers for the correct base address. For the newly loaded GDT segments.
    mov     es, ax          ; Move the data segment address into es.
    mov     fs, ax          ; Move the data segment address into fs.
    mov     gs, ax          ; Move the data segment address into gs.
    mov     ss, ax          ; Move the data segment address into ss.
    jmp     0x08:flush      ; Load 0x08 (Kernel code segment address) into segment CS, and flush address into EIP. (Far jump).

flush:
    ret

[global _load_IDT]
_load_IDT:
    lidt    [_idt_table]    ; Load the address of the IDT table into the IDTR.
    sti                     ; Enable interrupts for the CPU.
    ret



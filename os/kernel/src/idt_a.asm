use32                               ; Tell the compiler to use 32 bit (Protected mode).

section .text

extern _handle_KB_interrupt         ; Keyboard handler

; Disable interrupts on ISR.
; Store all General purpose registers onto kernel stack.

%macro _init_interrupt 0
    cli                             ; Disable interrupts.
    cld                             ; Clear direction flag.
    push    es                      ; Push es segment register onto stack.
    push    ds                      ; Push ds segment register onto stack.
    push    eax                     ; Push eax register onto stack.
    push    ebp                     ; Push ebp register onto stack.
    push    edi                     ; Push edi register onto stack.
    push    esi                     ; Push esi register onto stack.
    push    edx                     ; Push edx register onto stack.
    push    ecx                     ; Push ecx register onto stack.
    push    ebx                     ; Push ebx register onto stack.
%endmacro

%macro _end_interrupt 0
    pop     ebx                     ; Pop from stack into ebx.
    pop     ecx                     ; Pop from stack into ecx.
    pop     edx                     ; Pop from stack into edx.
    pop     esi                     ; Pop from stack into esi.
    pop     edi                     ; Pop from stack into edi.
    pop     ebp                     ; Pop from stack into ebp.
    pop     eax                     ; Pop from stack into eax.
    pop     ds                      ; Pop from stack into ds segment register.
    pop     es                      ; Pop from stack into es segment register.
    sti                             ; Enable interrupts.
    iretd                           ; Interrupt return, popping EIP, CS, EFLAGS & ERROR CODES back into their appropiate registers. For user space program to continue.
%endmacro

[global _KB_handler]
_KB_handler:
    _init_interrupt
    call    _handle_KB_interrupt    ; Call the C function to handle Keyboard interrupts.
    _end_interrupt

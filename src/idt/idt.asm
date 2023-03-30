section .asm

extern int21h_handler
extern no_interrupt_handler

global int21h
global idt_load
global no_interrupt

idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    lidt [ebx]          ; load to the interrupt descriptor table
    pop ebp
    ret

int21h:
    cli                 ; Clear interrupt flag; interrupts disabled when interrupt flag cleared.
    pushad              ; push the content of general purpose registers on the stack
                        ; with following order:
                        ; EAX, ECX, EDX, EBX, EBP, ESP ...
                        ; PUSHA is used for 16 bit instruction, and PUSHAD is used for 32 bit
    call int21h_handler
    popad               ; perform the reverse operation of the PUSHAD
    sti                 ; Set interrupt flag; external, maskable interrupts enabled
                        ; at the end of the next instruction.
    iret                ; interrupt return

no_interrupt:
    cli
    pushad
    call no_interrupt_handler
    popad
    sti
    iret
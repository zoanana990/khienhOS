section .asm

global insb
global insw
global outb
global outw

; refer to x86 instruction set reference
; input from port
insb:
    push ebp

    mov ebp, esp
    xor eax, eax
    mov edx, [ebp+8]
    in al, dx ; input byte from IO port

    pop ebp
    ret

insw:
    push ebp
    mov ebp, esp
    xor eax, eax
    mov edx, [ebp+8]
    in ax, dx ; input word from IO port
    pop ebp
    ret

outb:
    push ebp
    mov ebp, esp
    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al
    pop ebp
    ret

outw:
    push ebp
    mov ebp, esp
    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax
    pop ebp
    ret
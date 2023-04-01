[BITS 32]

section .asm

global paging_load_directory
global enable_paging

paging_load_directory:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax        ; copy the memory address to CR3
    pop ebp
    ret

enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0        ; copy the cr0 value
    or eax, 0x80000000  ; write 31 bit to enable paging function
    mov cr0, eax        ; copy the value to cr0
    pop ebp
    ret

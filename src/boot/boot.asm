ORG  0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start   ;code segment 
    nop

times 33 db 0         ; padding 0 for bias override

; some bias format reserved some offset, 
; if I do not reserved the address
; bias may override the code, and it will crash our startup code
; Thus, we rewrite this 

start:
    jmp 0:step2

step2:
    cli             ; clear interrupt
    mov ax, 0x00    ; segment address
    mov ds, ax      ; data segment = 0x7c0
    mov es, ax      ; extra segment = 0x7c0
    mov ss, ax
    mov sp, 0x7c00  ; stack pointer is 0x7c00 (0x7c0 * 16)
    sti             ; Enables Interrupt

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32


; GDT
gdt_start:
; This is just a NULL descriptor
gdt_null:
    dd 0x0
    dd 0x0

; please check the gdt register to know how is the code work
; offset 0x8
gdt_code:           ; CS should point to this
    dw 0xffff       ; Segment limit first 15 bit
    dw 0            ; Base first 0-15 bit
    db 0            ; Base 16-23 bit
    db 0x9a         ; Access byte
    db 11001111b    ; high 4 bit flags and low 4 bit flags
    db 0            ; base 24-31 bit

; offset 0x10
gdt_data:           ; DS, SS, ES, FS, GS
    dw 0xffff       ; Segment limit first 15 bit
    dw 0            ; Base first 0-15 bit
    db 0            ; Base 16-23 bit
    db 0x92         ; Access byte
    db 11001111b    ; high 4 bit flags and low 4 bit flags
    db 0            ; base 24-31 bit

gdt_end:

gdt_descriptor:     
    dw gdt_end - gdt_start - 1  ; size of gdt table
    dd gdt_start                ; offset of gdt table

[BITS 32]
load32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    jmp $

times 510-($-$$) db 0
dw 0xAA55

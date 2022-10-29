ORG  0
BITS 16

_start:
    jmp short start   ;code segment 
    nop

times 33 db 0         ; padding 0 for bias override

; some bias format reserved some offset, 
; if I don't reserved the address
; bias may override the code, and it will crash our startup code
; Thus, we rewrite this 

start:
    jmp 0x7c0:step2

handle_zero:
    mov ah, 0eh
    mov al, 'A'
    mov bx, 0x00
    int 0x10
    iret            ; return by interrupt

step2:
    cli             ; clear interrupt
    mov ax, 0x7c0   ; segment address
    mov ds, ax      ; data segment = 0x7c0
    mov es, ax      ; extra segment = 0x7c0
    mov ax, 0x00  
    mov ss, ax
    mov sp, 0x7c00  ; stack pointer is 0x7c00 (0x7c0 * 16)
    sti             ; Enables Interrupt

    mov word[ss:0x00], handle_zero
    mov word[ss:0x02], 0x7c0

    int 0

    mov si, message
    call print
    jmp $       
print:
    mov bx, 0   

.loop:
    lodsb      
    cmp al, 0
    je .done
    call print_char
    jmp .loop

.done:
    ret

print_char:
    mov ah, 0eh 
    int 0x10    
    ret

message: db 'Hello World', 0

times 510-($-$$) db 0

dw 0xAA55
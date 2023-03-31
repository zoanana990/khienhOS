ORG 0x7c00      ; Original memory address
                ; Usually is zero, and we need to jump to 0x7c00
BITS 16         ; 16 bit architecture

start:
    mov si, message
    call print
    jmp $       ; jump to label
                ; change the program counter register
                ; jump $ is infinite loop

print:
    mov bx, 0   

.loop:
    lodsb       ; load the data to al, then si-1
    cmp al, 0
    je .done
    call print_char
    jmp .loop

.done:
    ret

print_char:
    mov ah, 0eh ; move 0eh to ah
                ; What is ah and al?
                ; ah is high byte in ax;
                ; al is low byte in ax;
    int 0x10    ; int means interrupt
                ; 0x10 is the interrupt to output to the screen
                ; 0x10 is BIOS routine, BIOS will deal with hardware
    ret

message: db 'Hello World', 0

times 510-($-$$) db 0
; $ means address of currently executed instruction
; $$ means the first address of the section. 
; $ - $$ will generate an offset
; times will padding the instruction until 510 (in this case)
; db means define byte Generally one byte
; Thus this instruction will padding zeros in those offset

dw 0xAA55   ; dw: define words Generally 2 bytes
            ; we use AA55 instead of 55AA
            ; because intel is little endian

; loadsb
;
; IP register contains the value of current address
; ES, DS, SS are segmented register
; DS is data segment register, that is, we can use DS
; with the offset of the address to find the data
; 
; for example
; mov ax, 0x300
; mov ds, ax
; mov [0xff], 0x30
; we can find 0x30 at 0x300+0xff
;
; SP is stack pointer
; push 0xffff 
; this will push the value to stack pointer (memory)
; if the initial value is 0xfffe
; after I push, I found that is in 0xfffc, we can find
; in memory 0xfffc, the data is 0xffff
; if I pop it out, 
; pop ax
; we can find that ax is 0xffff
; call is also using stack pointer, to store return address
; Little endian: push 0xfff4 store 0xf4ff
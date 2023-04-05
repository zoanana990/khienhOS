ORG  0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


jmp short start   ;code segment
nop

; FAT16 Header
OEMIdentifier           db 'KHIENH  '
BytesPerSector          dw 0x200
SectorsPerCluster       db 0x80
ReservedSectors         dw 200
FATCopies               db 0x02
RootDirEntries          dw 0x40
NumSectors              dw 0x00
MediaType               db 0xF8
SectorsPerFat           dw 0x100
SectorsPerTrack         dw 0x20
NumberOfHeads           dw 0x40
HiddenSectors           dd 0x00
SectorsBig              dd 0x773594

; Extended BPB (Dos 4.0)
DriveNumber             db 0x80
WinNTBit                db 0x00
Signature               db 0x29
VolumeID                dd 0xD105
VolumeIDString          db 'PEACHOS BOO'
SystemIDString          db 'FAT16   '

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
    lgdt [gdt_descriptor]
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


; actually copy from the osdev.org
; load the kernel to the memory
[BITS 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SEG:0x0100000      ; jump to 1M

; This is a temporary method to load the kernel
ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

    ; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0

    ; insw is input word from io port specified into memory location
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret

times 510-($-$$) db 0
dw 0xAA55
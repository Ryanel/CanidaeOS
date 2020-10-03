; Multiboot Header
; -----------------------------------------------------------------------------
section .multiboot

MBALIGN  equ  1 << 0                ; Align modules to pages
MEMINFO  equ  1 << 1                ; Ask for memory map
FLAGS    equ  MBALIGN | MEMINFO     ; Multiboot flags
MAGIC    equ  0x1BADB002            ; Magic Number
CHECKSUM equ -(MAGIC + FLAGS)       ; Checksum
 
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

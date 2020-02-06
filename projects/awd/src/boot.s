; -----------------------------------------------------------------------------
; boot.s
; (C) 2020 Corwin McKnight
; Purpose: Setup the program into a stable state, then call awd_main
; -----------------------------------------------------------------------------

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

; Initial Stack
; -----------------------------------------------------------------------------
section .bss

align 16
; 8KB of Stack
stack_bottom:
    resb 8192 
stack_top:

; Page Tables
global paging_pml4
global paging_kernel_p3
global paging_kernel_p2

align 4096
paging_pml4:
    resb 4096
paging_kernel_p3:
    resb 4096
paging_kernel_p2:
    resb 4096

; Bootstub
; -----------------------------------------------------------------------------
section .text

extern awd_main
global _start:function (_start.end - _start)

_start:
    cli                             ; Prevent any interrupts
    mov     esp, stack_top          ; Setup Stack
    mov     ebp, stack_top
    push    ebx                     ; Push multiboot structure as a param
    push    eax                     ; Push magic
    call    awd_main                ; Start AWD
.end:

global paging_enable

paging_enable:
    mov eax, paging_pml4
    mov cr3, eax

    ; enable PAE-flag in cr4 (Physical Address Extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; set the long mode bit in the EFER MSR (model specific register)
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging in the cr0 register
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax
    ret
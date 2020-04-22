; -----------------------------------------------------------------------------
; enter.s
; (C) 2020 Corwin McKnight
; Purpose: Enter the kernel
; -----------------------------------------------------------------------------
; rodata
; -----------------------------------------------------------------------------

section .rodata
align 8
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64 ; new
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; Code Segment
.data_seg: equ $ - gdt64
    dq (1<<44) | (1<<47) | (1<<41)           ; Data segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

; Data
; -----------------------------------------------------------------------------
section .data
global k_ptr
align 8
k_ptr:
	dq	0x400000

; text
; -----------------------------------------------------------------------------
section .text
global enter_kernel

enter_kernel:
    lgdt [gdt64.pointer]
    jmp gdt64.code:longmode_enter_kernel

; 64-bit
; -----------------------------------------------------------------------------
bits 64
global long_mode_start
extern awd_boot_info
longmode_enter_kernel:
    mov rbx, awd_boot_info
    mov rax, [k_ptr]
    jmp rax
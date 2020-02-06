bits 64

; First thread stack
; -----------------------------------------------------------------------------
section .bss

align 16
stack_bottom:
    resb 16384 ; 16KB of Stack
stack_top:

; Text
; -----------------------------------------------------------------------------

section .text
global _start
extern kernel_entry

_start:
    mov     rsp, stack_top          ; Setup Stack
    mov     rbp, stack_top
    
    mov rdi, rbx

    call kernel_entry
    cli
    hlt
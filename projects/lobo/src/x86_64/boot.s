bits 64

; First thread stack
; -----------------------------------------------------------------------------
section .bss
align 16
stack_bottom:
    resb 16384 ; 16KB of Stack
stack_top:

; GDT.
section .rodata
align 8
gdt64:
    dq 0 ; zero entry
.code: equ $ - gdt64 ; new
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; Code segment
.data_seg: equ $ - gdt64
    dq (1<<44) | (1<<47) | (1<<41)           ; Data segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

; Text
; -----------------------------------------------------------------------------

section .text
global _start
extern kernel_entry

_start:
    mov     rsp, stack_top      ; Setup Stack
    mov     rbp, stack_top 
    mov rdi, rbx                ; Set the first argument for kernel_entry to
                                ; the struct passed by awd_info
    lgdt [gdt64.pointer]        ; Switch GDT, CS should still be 0x08
    push rax                    ; Switch segments
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    pop rax                     ; Restore state.
    call kernel_entry           ; Enter the kernel!
.loop:
    cli
    hlt
    jmp .loop
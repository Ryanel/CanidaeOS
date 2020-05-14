bits 64

; First thread stack
; -----------------------------------------------------------------------------
section .bss
align 16
stack_bottom:
    resb 16384 ; 16KB of Stack
stack_top:

; TSS

; GDT.
section .rodata
align 8
gdt64:
.Null: equ $ - gdt64         ; The null descriptor.
    dw 0xFFFF                    ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 0                         ; Access.
    db 1                         ; Granularity.
    db 0                         ; Base (high).
    .Code: equ $ - gdt64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10011010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .Data: equ $ - gdt64         ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 10010010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
    .CodeUser: equ $ - gdt64         ; The code descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11111010b                 ; Access (exec/read).
    db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
    db 0                         ; Base (high).
    .DataUser: equ $ - gdt64     ; The data descriptor.
    dw 0                         ; Limit (low).
    dw 0                         ; Base (low).
    db 0                         ; Base (middle)
    db 11110010b                 ; Access (read/write).
    db 00000000b                 ; Granularity.
    db 0                         ; Base (high).
.pointer:
    dw $ - gdt64 - 1
    dq gdt64

; Text
; -----------------------------------------------------------------------------

section .text
global _start
extern kernel_entry
extern _init

_start:
    mov     rsp, stack_top      ; Setup Stack
    mov     rbp, stack_top 
    mov rdi, rbx                ; Set the first argument for kernel_entry to
                                ; the struct passed by awd_info
    lgdt [gdt64.pointer]        ; Switch GDT
    push rax                    ; Switch segments, CS should still be 0x08
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    pop rax                     ; Restore state.
    push rdi
    call _init                  ; Now, call init!
    pop rdi
    call kernel_entry           ; Enter the kernel!
.loop:
    cli
    hlt
    jmp .loop
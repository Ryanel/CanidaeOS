bits 64

extern gtd64
extern gdt64_pointer
extern kernel_early
extern _init

; Boot thread information
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
_start:
    mov     rsp, stack_top      ; Setup Stack
    mov     rbp, stack_top 
    mov rdi, rbx                ; Set the first argument for kernel_entry to
                                ; the struct passed by awd_info
    lgdt [gdt64_pointer]        ; Switch GDT
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
    call kernel_early           ; Enter the kernel!
.loop:
    cli
    hlt
    jmp .loop
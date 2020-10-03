; -----------------------------------------------------------------------------
; boot.s
; (C) 2020 Corwin McKnight
; Purpose: Get AWD into a stable state, then call main
; -----------------------------------------------------------------------------

section .text

; Imports
extern stack_top  ; from stack.s
extern awd_main   ; from main.c

; Global Declarations
global   _start:function (_start.end - _start)
global awd_halt:function (awd_halt.end - awd_halt)

; First code run when bootloader passes control.
_start:
    cli ; Clear interrupts
    
    ; Setup Stack
    mov  esp, stack_top
    mov  ebp, stack_top

    ; Push parameters
    push ebx
    push eax

    ; Call the main function.
    call awd_main
    jmp  awd_halt
.end:

; Halt the computer in an infinite loop
awd_halt:
    cli  ; Clear interrupts
.loop:
    hlt
    jmp  .loop
.end:
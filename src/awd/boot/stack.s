section .bss

global stack_top
global stack_bottom

align 16
; 8KB of Stack
stack_bottom:
    resb 8192 
stack_top:

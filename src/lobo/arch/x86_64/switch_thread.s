global switch_thread

; rdi = address of new thread TCB
; rsi = address of current TCB
switch_thread:
    ; Save current thread state
    push rax
    push rcx
    push rdx
    push rbx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    ; Save current stack pointer...
    mov qword [rsi], rsp

    ; Load next thread state
    mov rsp, [rdi]              ; Switch stacks
    mov rcx, [rdi + (0x8 * 1)]  ; Load new tasks VAS physical address
    mov rax, cr3                ; Load current threads VAS physical address
    cmp rax, rcx                ; Check if New VAS == Old VAS

    je .doneSwitchVAS           ; Don't flush TLB if they are the same
    mov cr3, rax                ; Switch to new thread's VAS, flush TLB

.doneSwitchVAS:
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rbx
    pop rdx
    pop rcx
    pop rax
    
    ret

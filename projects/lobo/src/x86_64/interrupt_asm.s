extern fault_handler
extern interrupt_handler

%macro interrupt_saveregs 0
    ; RIP, CS, RFLAGS, RSP, and SS are pushed already. IRETQ restores these.
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
%endmacro

%macro interrupt_loadregs 0
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
%endmacro

%macro isr_handler 2
global %1
%1:
    push byte 0             ; Error Code
    push byte %2            ; Int Number
    jmp isr_common
%endmacro

%macro isr_handler_error 2
global %1
%1:
                            ; Error Pushed By CPU already
    push byte %2            ; Int Number
    jmp isr_common
%endmacro

%macro irq_handler 2
global %1
%1:
    push byte 0             ; Error Code
    push byte %2            ; Int Number
    jmp irq_common
%endmacro

isr_common:
    interrupt_saveregs      ; Save Register File
    mov rdi, rsp            ; Make the first argument to interrupt_handler the register file
    mov rbp, 0              ; New Stack Frame
    cld                     ; SysV ABI compliance, clear DF
    call fault_handler      ; Call into kernel
    interrupt_loadregs      ; Load Register File
    add rsp, 16             ; Error Code + IRQ number cleanup
    iretq                   ; Return

irq_common:
    interrupt_saveregs      ; Save Register File
    mov rdi, rsp            ; Make the first argument to interrupt_handler the register file
    mov rbp, 0              ; New Stack Frame
    cld                     ; SysV ABI compliance, clear DF
    call interrupt_handler  ; Call into kernel
    interrupt_loadregs      ; Load Register File
    add rsp, 16             ; Error Code + IRQ number cleanup
    iretq                   ; Return

global interrupt_set_idt
interrupt_set_idt:
    lidt [rdi]
    ret

isr_handler         interrupt_isr0,  0
isr_handler         interrupt_isr1,  1
isr_handler         interrupt_isr2,  2
isr_handler         interrupt_isr3,  3
isr_handler         interrupt_isr4,  4
isr_handler         interrupt_isr5,  5
isr_handler         interrupt_isr6,  6
isr_handler         interrupt_isr7,  7
isr_handler_error   interrupt_isr8,  8
isr_handler         interrupt_isr9,  9
isr_handler_error   interrupt_isr10, 10
isr_handler_error   interrupt_isr11, 11
isr_handler_error   interrupt_isr12, 12
isr_handler_error   interrupt_isr13, 13
isr_handler_error   interrupt_isr14, 14
isr_handler         interrupt_isr15, 15
isr_handler         interrupt_isr16, 16
isr_handler_error   interrupt_isr17, 17
isr_handler         interrupt_isr18, 18
isr_handler         interrupt_isr19, 19
isr_handler         interrupt_isr20, 20
isr_handler_error   interrupt_isr21, 21
isr_handler         interrupt_isr22, 22
isr_handler         interrupt_isr23, 23
isr_handler         interrupt_isr24, 24
isr_handler         interrupt_isr25, 25
isr_handler         interrupt_isr26, 26
isr_handler         interrupt_isr27, 27
isr_handler         interrupt_isr28, 28
isr_handler         interrupt_isr29, 29
isr_handler_error   interrupt_isr30, 30
isr_handler         interrupt_isr31, 31

irq_handler         interrupt_irq0,  32
irq_handler         interrupt_irq1,  33
irq_handler         interrupt_irq2,  34
irq_handler         interrupt_irq3,  35
irq_handler         interrupt_irq4,  36
irq_handler         interrupt_irq5,  37
irq_handler         interrupt_irq6,  38
irq_handler         interrupt_irq7,  39
irq_handler         interrupt_irq8,  40
irq_handler         interrupt_irq9,  41
irq_handler         interrupt_irq10, 42
irq_handler         interrupt_irq11, 43
irq_handler         interrupt_irq12, 44
irq_handler         interrupt_irq13, 45
irq_handler         interrupt_irq14, 46
irq_handler         interrupt_irq15, 47
#pragma once

#include <stdint.h>

#define IDT_ENTRY_ATTR_PRESENT 0x80
#define IDT_ENTRY_ATTR_DPL0 0x0  // Unsure if this is valid...
#define IDT_ENTRY_ATTR_DPL3 0x60
#define IDT_ENTRY_ATTR_INTERRUPT 0x5

/// IDT Entry
typedef struct IDTTableEntry {
    uint16_t offset_low;
    uint16_t cs_selector;
    uint8_t  ist;
    uint8_t  attributes;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t zero_2;
} __attribute__((__packed__)) idt_table_entry_t;

/// Pointer to IDT
typedef struct IDTTablePointer {
    uint16_t limit;
    uint64_t address;
} __attribute__((__packed__)) idt_table_ptr_t;

struct InterruptContext {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8; // Saved Regs
    uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;    // Saved Regs
    uint64_t int_no, err_code;                     // Pushed from Stub
    uint64_t rip, cs, eflags, userrsp, ss;         // Pushed by CPU
};

extern "C" void interrupt_set_idt(uint64_t idtr);

void init_idt();
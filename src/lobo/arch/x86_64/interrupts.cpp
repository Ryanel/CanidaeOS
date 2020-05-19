#include <kernel/arch/x86_64/interrupt_methods.h>
#include <kernel/arch/x86_64/interrupts.h>
#include <kernel/arch/x86_64/ports.h>
#include <kernel/cpu.h>
#include <kernel/log.h>
#include <kernel/scheduler.h>
#include <string.h>

idt_table_entry_t table_idt[256] __attribute__((aligned(0x1000)));
idt_table_ptr_t   table_idt_ptr;

using namespace kernel;

const char* exception_strings[] = {"Divide By Zero",
                                   "Debug",
                                   "NMI",
                                   "Breakpoint",
                                   "Overflow",
                                   "Bound Range Exceeded",
                                   "Invalid Opcode",
                                   "Device Not Avalable",
                                   "Double Falt",
                                   "Coprocessor Segment Overrun",
                                   "Invalid TSS",
                                   "Segment Not Present",
                                   "Stack-Segment Fault",
                                   "General Protection Fault",
                                   "Page Fault",
                                   "Reserved",
                                   "x87 Floating-Point Exception",
                                   "Alignment Check",
                                   "Machine Check",
                                   "SIMD Floating-Point Exception",
                                   "Virtualization Exception",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Reserved",
                                   "Security Exception",
                                   "Reserved"};

extern "C" void fault_handler(struct InterruptContext* r) {
    auto& log = log::Get();
    // KernelLog::Get().Log("int", "Caught an interrupt! %d, error %p", r->int_no, r->err_code);

    if (r->int_no < 32) {
        log.LogRaw("%35sException\n", "");
        log.Log("fault", "%s (int %d), Error Code: 0x%08x", exception_strings[r->int_no], r->int_no,
                (uint32_t)r->err_code);
        log.Log("fault", "RBP:0x%016p RSP:0x%016p RIP:0x%016p", r->rbp, r->userrsp, r->rip);
        log.Log("fault", "RAX:0x%016p RBX:0x%016p RCX:0x%016p", r->rax, r->rbx, r->rcx);
        log.Log("fault", "RDX:0x%016p RDI:0x%016p RSI:0x%016p", r->rdx, r->rdi, r->rsi);
        log.Log("fault", " R8:0x%016p  R9:0x%016p R10:0x%016p", r->r8, r->r9, r->r10);
        log.Log("fault", "R11:0x%016p R12:0x%016p R13:0x%016p", r->r11, r->r12, r->r13);
        log.Log("fault", "R14:0x%016p R15:0x%016p", r->r14, r->r15);

        if (r->int_no == 14) {
            // Page fault
            uint64_t faultingAddress;
            asm volatile("mov %%cr2, %0" : "=r"(faultingAddress));
            log.Log("fault", "Faulting address: 0x%016p\n", faultingAddress);
        }

        kernel::cpu::HaltCPU();
    }
}

extern "C" void interrupt_handler(struct InterruptContext* r) {
    auto& log            = log::Get();
    bool  eoi_sent_early = false;

    // Print out any IRQ that's not IRQ 0
    if (r->int_no != 32) {
        log.Log("int", "Recieved IRQ %d, handled.", (r->int_no - 32));
    } else {
        // Timer interrupt
        eoi_sent_early = true;

        if (r->int_no >= 40) { outb(0xA0, 0x20); }
        outb(0x20, 0x20);

        asm("sti");  // Enable interrupts again, so we can recieve an interrupt
        kernel::scheduling::Scheduler::Get().Schedule();
    }

    // End of Interrupt to PIC (and slave)
    if (!eoi_sent_early) {
        if (r->int_no >= 40) { outb(0xA0, 0x20); }
        outb(0x20, 0x20);
    }
}

idt_table_entry_t idt_populate_entry(uint64_t entry, uint16_t cs_selector, uint8_t attributes) {
    idt_table_entry_t idt_entry;

    idt_entry.offset_low  = entry & 0xFFFF;
    idt_entry.offset_mid  = (entry & 0xFFFF0000) >> 16;
    idt_entry.offset_hi   = (entry & 0xFFFFFFFF00000000) >> 32;
    idt_entry.cs_selector = cs_selector;
    idt_entry.attributes  = attributes;
    // Already memset to zero
    // idt_entry.zero_2 = 0;
    // idt_entry.ist    = 0;
    return idt_entry;
}

void init_idt() {
    auto& kernelLog = log::Get();
    kernelLog.Log("int", "Initialising interrupts");
    // Fill with non-present data
    memset(&table_idt, 0, sizeof(idt_table_entry_t) * 256);

    // Remap the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // ISR's -------------------------------------------------------------------
    table_idt[0]  = idt_populate_entry((uint64_t)(&interrupt_isr0), 0x08, 0x8E);
    table_idt[1]  = idt_populate_entry((uint64_t)(&interrupt_isr1), 0x08, 0x8E);
    table_idt[2]  = idt_populate_entry((uint64_t)(&interrupt_isr2), 0x08, 0x8E);
    table_idt[3]  = idt_populate_entry((uint64_t)(&interrupt_isr3), 0x08, 0x8E);
    table_idt[4]  = idt_populate_entry((uint64_t)(&interrupt_isr4), 0x08, 0x8E);
    table_idt[5]  = idt_populate_entry((uint64_t)(&interrupt_isr5), 0x08, 0x8E);
    table_idt[6]  = idt_populate_entry((uint64_t)(&interrupt_isr6), 0x08, 0x8E);
    table_idt[7]  = idt_populate_entry((uint64_t)(&interrupt_isr7), 0x08, 0x8E);
    table_idt[8]  = idt_populate_entry((uint64_t)(&interrupt_isr8), 0x08, 0x8E);
    table_idt[9]  = idt_populate_entry((uint64_t)(&interrupt_isr9), 0x08, 0x8E);
    table_idt[10] = idt_populate_entry((uint64_t)(&interrupt_isr10), 0x08, 0x8E);
    table_idt[11] = idt_populate_entry((uint64_t)(&interrupt_isr11), 0x08, 0x8E);
    table_idt[12] = idt_populate_entry((uint64_t)(&interrupt_isr12), 0x08, 0x8E);
    table_idt[13] = idt_populate_entry((uint64_t)(&interrupt_isr13), 0x08, 0x8E);
    table_idt[14] = idt_populate_entry((uint64_t)(&interrupt_isr14), 0x08, 0x8E);
    table_idt[15] = idt_populate_entry((uint64_t)(&interrupt_isr15), 0x08, 0x8E);
    table_idt[16] = idt_populate_entry((uint64_t)(&interrupt_isr16), 0x08, 0x8E);
    table_idt[17] = idt_populate_entry((uint64_t)(&interrupt_isr17), 0x08, 0x8E);
    table_idt[18] = idt_populate_entry((uint64_t)(&interrupt_isr18), 0x08, 0x8E);
    table_idt[19] = idt_populate_entry((uint64_t)(&interrupt_isr19), 0x08, 0x8E);
    table_idt[20] = idt_populate_entry((uint64_t)(&interrupt_isr20), 0x08, 0x8E);
    table_idt[21] = idt_populate_entry((uint64_t)(&interrupt_isr21), 0x08, 0x8E);
    table_idt[22] = idt_populate_entry((uint64_t)(&interrupt_isr22), 0x08, 0x8E);
    table_idt[23] = idt_populate_entry((uint64_t)(&interrupt_isr23), 0x08, 0x8E);
    table_idt[24] = idt_populate_entry((uint64_t)(&interrupt_isr24), 0x08, 0x8E);
    table_idt[25] = idt_populate_entry((uint64_t)(&interrupt_isr25), 0x08, 0x8E);
    table_idt[26] = idt_populate_entry((uint64_t)(&interrupt_isr26), 0x08, 0x8E);
    table_idt[27] = idt_populate_entry((uint64_t)(&interrupt_isr27), 0x08, 0x8E);
    table_idt[28] = idt_populate_entry((uint64_t)(&interrupt_isr28), 0x08, 0x8E);
    table_idt[29] = idt_populate_entry((uint64_t)(&interrupt_isr29), 0x08, 0x8E);
    table_idt[30] = idt_populate_entry((uint64_t)(&interrupt_isr30), 0x08, 0x8E);
    table_idt[31] = idt_populate_entry((uint64_t)(&interrupt_isr31), 0x08, 0x8E);

    // IRQ's --------------------------------------------------------------------
    table_idt[32] = idt_populate_entry((uint64_t)(&interrupt_irq0), 0x08, 0x8E);
    table_idt[33] = idt_populate_entry((uint64_t)(&interrupt_irq1), 0x08, 0x8E);
    table_idt[34] = idt_populate_entry((uint64_t)(&interrupt_irq2), 0x08, 0x8E);
    table_idt[35] = idt_populate_entry((uint64_t)(&interrupt_irq3), 0x08, 0x8E);
    table_idt[36] = idt_populate_entry((uint64_t)(&interrupt_irq4), 0x08, 0x8E);
    table_idt[37] = idt_populate_entry((uint64_t)(&interrupt_irq5), 0x08, 0x8E);
    table_idt[38] = idt_populate_entry((uint64_t)(&interrupt_irq6), 0x08, 0x8E);
    table_idt[39] = idt_populate_entry((uint64_t)(&interrupt_irq7), 0x08, 0x8E);
    table_idt[40] = idt_populate_entry((uint64_t)(&interrupt_irq8), 0x08, 0x8E);
    table_idt[41] = idt_populate_entry((uint64_t)(&interrupt_irq9), 0x08, 0x8E);
    table_idt[42] = idt_populate_entry((uint64_t)(&interrupt_irq10), 0x08, 0x8E);
    table_idt[43] = idt_populate_entry((uint64_t)(&interrupt_irq11), 0x08, 0x8E);
    table_idt[44] = idt_populate_entry((uint64_t)(&interrupt_irq12), 0x08, 0x8E);
    table_idt[45] = idt_populate_entry((uint64_t)(&interrupt_irq13), 0x08, 0x8E);
    table_idt[46] = idt_populate_entry((uint64_t)(&interrupt_irq14), 0x08, 0x8E);
    table_idt[47] = idt_populate_entry((uint64_t)(&interrupt_irq15), 0x08, 0x8E);

    // Flush the IDT
    table_idt_ptr.address = (uint64_t)(&table_idt);
    table_idt_ptr.limit   = (sizeof(idt_table_entry_t) * 256) - 1;
    interrupt_set_idt((uint64_t)(&table_idt_ptr));

    kernelLog.Log("int", "Interrupts enabled");

    asm("sti");  // Start interrupts
}

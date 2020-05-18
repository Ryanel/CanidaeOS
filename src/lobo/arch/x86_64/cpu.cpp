#include "kernel/cpu.h"

void kernel::cpu::IdleLoop() {
    while(true) {
        asm("hlt");
    }
}

void kernel::cpu::HaltCPU() {
    asm("cli");
    asm("hlt");
}
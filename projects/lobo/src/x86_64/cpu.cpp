#include "kernel/cpu.h"

void Kernel::CPU::IdleLoop() {
    while(true) {
        asm("hlt");
    }
}

void Kernel::CPU::HaltCPU() {
    asm("cli");
    asm("hlt");
}
#pragma once

#include <stddef.h>
#include <stdint.h>

#define TASK_FLAG_DEAD 0x01

class ThreadControlBlock {
   public:
    uint64_t    stack_top;         // The Virtual Address of the Stack 
    uint64_t    vas;               // Virtual address space, page table.
    uint64_t    flags;             // Flags
    const char* taskName;
    ThreadControlBlock* next;
};
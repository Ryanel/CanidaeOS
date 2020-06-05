#pragma once

#include <stddef.h>
#include <stdint.h>

namespace kernel {
namespace scheduling {

/// Flags
constexpr int TCB_FLAG_DEAD = 0x01; // Is the thread dead?

class ThreadControlBlock {
   public:
    uint64_t            stack_top;  // The Virtual Address of the Stack
    uint64_t            vas;        // Virtual address space, page table.
    uint64_t            flags;      // Flags
    const char*         taskName;
};

};  // namespace Scheduling
}  // namespace Kernel

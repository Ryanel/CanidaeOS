#pragma once

#include <stddef.h>
#include <stdint.h>

namespace kernel {
namespace scheduling {

/// Flags
enum class TCBState {
    Ready,
    Running,
    Waiting,
    Dead
};

class ThreadControlBlock {
   public:
    uint64_t            stack_top;  ///< The Virtual Address of the Stack
    uint64_t            vas;        ///< Virtual address space, page table.
    TCBState            state;      ///< Flags
    const char*         taskName;   ///< Thread name
    uint64_t            threadID;   ///< ID of the thread
};

};  // namespace Scheduling
}  // namespace Kernel

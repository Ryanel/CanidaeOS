#pragma once

#include <stddef.h>
#include <stdint.h>

namespace kernel {
namespace scheduling {

/// Flags
enum class TCBState { Ready, Running, Waiting, Dead };

/// A thread is an execution context within a process.
class ThreadControlBlock {
   public:
    uint64_t    stack_top;   ///< The Virtual Address of the Stack
    //uint64_t    vas;         ///< Virtual address space, page table. //TODO: Get rid of this! Should use Task's VAS.
    TCBState    state;       ///< Flags
    const char* threadName;  ///< Thread name
    uint64_t    threadID;    ///< ID of the thread
    uint64_t    taskID;      ///< ID of the parent task
};

/// A task is a collecton of resources, analogous to a process
class TaskControlBlock {
   public:
    uint64_t    vas;       ///< Virtual address space of the process, usually a page table.
    uint64_t    taskID;    ///< ID of the thread
    TCBState    state;     ///< State of Task
    const char* taskName;  ///< Thread name
};

};  // namespace scheduling
}  // namespace kernel

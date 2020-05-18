#pragma once

#include "kernel/task.h"

namespace kernel {
namespace scheduling {

class Scheduler {
   public:
    ThreadControlBlock  idleThread;
    ThreadControlBlock* currentThread;

    bool schedulingEnabled = false;

   public:
    static Scheduler&   Get();
    void                Init();
    void                OnTick();
    ThreadControlBlock* CreateThread(const char* name, void* function);
    void                SwitchThread(ThreadControlBlock* newThread);
    void                PrintTaskInfo(ThreadControlBlock* tcb);
    void                EnableScheduling();
    void                DisableScheduling();
    void                Schedule();  // Switch to the next thread.
};
}  // namespace Scheduling
}  // namespace Kernel